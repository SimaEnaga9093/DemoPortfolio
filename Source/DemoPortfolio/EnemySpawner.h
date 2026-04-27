// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class APawn;
class APatrolPath;
class UDataTable;

class UBillboardComponent;
class UTextRenderComponent;
class UArrowComponent;

struct FEnemySpawnGroupRow;

/**
 * 에너미가 소환되는 지점을 관리하는 액터
 */
UCLASS()
class DEMOPORTFOLIO_API AEnemySpawner : public AActor
{
    GENERATED_BODY()

public:
    AEnemySpawner();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // 활성화 거리 표기를 위한 에디터 뷰포트에서도 Tick이 돌도록 허용
    virtual bool ShouldTickIfViewportsOnly() const override { return true; }

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
    /**
    * 스포너가 소환할 에너미 스폰 그룹 테이블
    * [SAMPLE] 실제 로직에서는 ConstructorHelpers 등을 사용해 사전에 로드된 데이터 테이블을 사용함.
    */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DemoPortfolio|Spawn")
    TObjectPtr<UDataTable> SpawnGroupTable;

    // 스폰 그룹의 아이디
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DemoPortfolio|Spawn")
    FName SpawnGroupId;

    // 이 스포너에서 소환된 에너미가 사용할 정찰 경로
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DemoPortfolio|Spawn")
    TObjectPtr<APatrolPath> PatrolPath;

    // 플레이어가 이 거리 안에 들어오면 스폰 된다.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DemoPortfolio|Spawn", meta = (ClampMin = "0.0"))
    float ActivationDistance = 500.f;

private:
    /**
    * 소환될 에너미 클래스
    * [SAMPLE] 실제 로직에서는 APawn -> ADPEnemyBase 등 에너미 클래스로 변경
    */
    UPROPERTY(Transient)
    TSoftClassPtr<APawn> SelectedEnemyClass;

    bool bHasSpawned = false;

    void CheckActivationAndSpawn();

    void SpawnEnemy();

    TSoftClassPtr<APawn> SelectEnemyFromRow(const FEnemySpawnGroupRow& Row) const;

#if WITH_EDITORONLY_DATA
    UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UBillboardComponent> EditorBillboard;

    UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UTextRenderComponent> EditorGroupIdText;

    UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UArrowComponent> EditorArrow;
#endif

#if WITH_EDITOR
    // 에디터 비주얼 갱신
    void UpdateEditorVisuals();
#endif
};
