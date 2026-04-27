// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

class APatrolPoint;

/**
 * 정찰 경로 매니저
 */
UCLASS()
class DEMOPORTFOLIO_API APatrolPath : public AActor
{
	GENERATED_BODY()

public:
	APatrolPath();

#if WITH_EDITOR
	// 자식 포인트 BP를 자신의 위치에 스폰하고 배열에 추가
	UFUNCTION(CallInEditor, Category = "DemoPortfolio")
	void AddPoint();

	// 노드 갱신 상황을 즉시 새로고침
	UFUNCTION(CallInEditor, Category = "DemoPortfolio")
	void RefreshPoints() { UpdateEditorVisuals(); }

	// 에디터 비주얼 갱신
	void UpdateEditorVisuals();
#endif

	// 위치 배열 반환
	TArray<FVector> GetPathLocations() const;

	const TArray<TObjectPtr<APatrolPoint>>& GetPoints() const { return Points; }

protected:
	// 순서대로 정렬된 정찰 포인트들
	UPROPERTY(VisibleAnywhere, Category = "DemoPortfolio|Patrol")
	TArray<TObjectPtr<APatrolPoint>> Points;
};
