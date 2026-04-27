// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnemySpawnGroupRow.generated.h"

class APawn;

/**
* 에너미를 스폰시키기 위한 그룹 데이터
 */
USTRUCT(BlueprintType)
struct DEMOPORTFOLIO_API FEnemySpawnGroupRow : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

    /**
    * Key : 에너미 액터 BP 클래스
    * Value : 가중치 (합산하여 사용됨)
    * 
    * [SAMPLE] 실제 로직에서는 APawn -> ADPEnemyBase 등 에너미 클래스로 변경
    * [SAMPLE] 실제 데이터에서는 SpawnGroupID 컬럼으로 구분하여 Excel 등 외부 프로그램에서 파싱하여 작업하기 편하게 제공됨
    */
    UPROPERTY(EditAnywhere, Category = "DemoPortfolio|Spawn", meta = (ForceInlineRow))
    TMap<TSoftClassPtr<APawn>, float> SpawnRatio;
};
