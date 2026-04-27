// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySpawner.h"

#include "DrawDebugHelpers.h"
#include "Engine/DataTable.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"

#include "EnemySpawnGroupRow.h"

#if WITH_EDITORONLY_DATA
#include "Components/BillboardComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/ArrowComponent.h"
#endif

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickInterval = 0.5f;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

#if WITH_EDITORONLY_DATA
	// 컴포넌트 세팅
	EditorBillboard = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("EditorBillboard"));
	if (EditorBillboard)
	{
		EditorBillboard->SetupAttachment(root);
		EditorBillboard->bIsEditorOnly = true;
	}

	EditorArrow = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("EditorArrow"));
	if (EditorArrow)
	{
		EditorArrow->SetupAttachment(root);
		EditorArrow->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
		EditorArrow->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
		EditorArrow->ArrowColor = FColor::Yellow;
		EditorArrow->bIsEditorOnly = true;
	}

	EditorGroupIdText = CreateEditorOnlyDefaultSubobject<UTextRenderComponent>(TEXT("EditorGroupIdText"));
	if (EditorGroupIdText)
	{
		EditorGroupIdText->SetupAttachment(root);
		EditorGroupIdText->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
		EditorGroupIdText->SetWorldRotation(FRotator(90.f, 0.f, 90.f));
		EditorGroupIdText->SetWorldSize(50.f);
		EditorGroupIdText->SetTextRenderColor(FColor::Yellow);
		EditorGroupIdText->SetHiddenInGame(true);
		EditorGroupIdText->bIsEditorOnly = true;
	}
#endif
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	// 시작 시점에 어떤 에너미를 스폰할지만 결정해둔다.
	if (SpawnGroupId.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("[AEnemySpawner] SpawnGroupId 미지정 : %s"), *GetName());
	}
	else if (FEnemySpawnGroupRow* row = SpawnGroupTable->FindRow<FEnemySpawnGroupRow>(SpawnGroupId, TEXT("AEnemySpawner")))
	{
		SelectedEnemyClass = SelectEnemyFromRow(*row);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[AEnemySpawner] SpawnGroupId '%s' 행 없음 : %s"), *SpawnGroupId.ToString(), *GetName());
	}

	// 선택에 실패했다면 Tick 끄기
	if (SelectedEnemyClass.IsNull())
	{
		SetActorTickEnabled(false);
	}
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckActivationAndSpawn();

#if WITH_EDITORONLY_DATA
	// 디버깅 중에만 범위 보여주기
	DrawDebugCircle(
		GetWorld(),
		GetActorLocation(),
		ActivationDistance,
		32,
		FColor::Yellow,
		false,
		0.6f,
		0,
		2.f,
		FVector(1.f, 0.f, 0.f),
		FVector(0.f, 1.f, 0.f),
		false
	);
#endif
}

void AEnemySpawner::CheckActivationAndSpawn()
{
	if (bHasSpawned || SelectedEnemyClass.IsNull())
	{
		return;
	}

	// [SAMPLE] 실제 프로젝트에서는 다수 플레이어를 대상으로 한 로직이 사용되었다.
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PlayerPawn)
	{
		return;
	}

	// 거리 확인
	float distance = FVector::DistSquared(GetActorLocation(), PlayerPawn->GetActorLocation());
	if (distance <= ActivationDistance * ActivationDistance)
	{
		SpawnEnemy();
	}
}

void AEnemySpawner::SpawnEnemy()
{
	UClass* EnemyClass = SelectedEnemyClass.LoadSynchronous();
	if (!EnemyClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AEnemySpawner] SpawnEnemy 로드 실패 : %s"), *SelectedEnemyClass.ToString());
		bHasSpawned = true;
		SetActorTickEnabled(false);
		return;
	}

	// 무조건 스폰 시킨다.
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	Params.Owner = this;

	APawn* SpawnedActor = GetWorld()->SpawnActor<APawn>(
		EnemyClass,
		GetActorLocation(),
		GetActorRotation(),
		Params);

	// [SAMPLE] 실제 프로젝트에서는 소환된 에너미에게 정찰 경로를 전달
	if (SpawnedActor && PatrolPath != nullptr)
	{
		// [SAMPLE] Spawned->SetPatrolPath(PatrolPath->GetPathLocations());
	}

	bHasSpawned = true;
	SetActorTickEnabled(false);
}

TSoftClassPtr<APawn> AEnemySpawner::SelectEnemyFromRow(const FEnemySpawnGroupRow& Row) const
{
	// 누적 가중치 합산
	float totalRatio = 0.f;
	for (TPair<TSoftClassPtr<APawn>, float> Pair : Row.SpawnRatio)
	{
		if (Pair.Value > 0.f && !Pair.Key.IsNull())
		{
			totalRatio += Pair.Value;
		}
	}

	if (totalRatio <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AEnemySpawner] SpawnGroup 가중치 0이하 : %s"), *SpawnGroupId.ToString());
		return nullptr;
	}

	// 누적 가중치로 랜덤한 대상 탐색
	float randomRatio = FMath::FRandRange(0.f, totalRatio);
	for (TPair<TSoftClassPtr<APawn>, float> Pair : Row.SpawnRatio)
	{
		if (Pair.Value <= 0.f || Pair.Key.IsNull())
		{
			continue;
		}

		randomRatio -= Pair.Value;
		if (randomRatio <= 0)
		{
			return Pair.Key;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[AEnemySpawner] SpawnGroup 미지정 : %s"), *SpawnGroupId.ToString());
	return nullptr;
}

#if WITH_EDITOR

void AEnemySpawner::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	UpdateEditorVisuals();
}

void AEnemySpawner::UpdateEditorVisuals()
{
#if WITH_EDITORONLY_DATA
	if (EditorGroupIdText)
	{
		FString text = SpawnGroupId.IsNone() ? TEXT("NULL ID") : SpawnGroupId.ToString();
		EditorGroupIdText->SetText(FText::FromString(text));
		EditorGroupIdText->SetTextRenderColor(SpawnGroupId.IsNone() ? FColor::Red : FColor::Yellow);
	}
#endif
}
#endif
