// Fill out your copyright notice in the Description page of Project Settings.

#include "PatrolPath.h"

#include "PatrolPoint.h"
#include "Engine/World.h"


APatrolPath::APatrolPath()
{
    PrimaryActorTick.bCanEverTick = false;

    USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(root);
}

TArray<FVector> APatrolPath::GetPathLocations() const
{
    TArray<FVector> locations;
    locations.Reserve(Points.Num());

    // 모든 포인트들 순서를 맞춰 기록 후 반환
    for (const APatrolPoint* point : Points)
    {
        if (IsValid(point))
        {
            locations.Add(point->GetActorLocation());
        }
    }

    return locations;
}

#if WITH_EDITOR
void APatrolPath::AddPoint()
{
    UWorld* world = GetWorld();
    if (!world || world->IsGameWorld())
        return;

    // 스폰 파라미터
    FActorSpawnParameters spawnParams;
    spawnParams.Owner = this;
    spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // 액터 소환
    APatrolPoint* newPoint = world->SpawnActor<APatrolPoint>(
        GetActorLocation(),
        FRotator::ZeroRotator,
        spawnParams);

    if (!newPoint)
        return;

    // 인덱스 보이도록 액터명도 수정
    newPoint->SetActorLabel(FString::Printf(TEXT("PatrolPoint_%d"), Points.Num()));
    newPoint->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
    newPoint->SetOwningPath(this);
    Points.Add(newPoint);

    UpdateEditorVisuals();
}

void APatrolPath::UpdateEditorVisuals()
{
    // 소실된 포인트 정리
    Points.RemoveAll([](const APatrolPoint* p) {
        return !IsValid(p);
    });

    for (APatrolPoint* point : Points)
    {
        point->UpdateEditorVisuals();
    }
}
#endif
