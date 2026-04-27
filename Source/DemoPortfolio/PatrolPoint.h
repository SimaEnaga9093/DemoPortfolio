// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPoint.generated.h"

class APatrolPath;
class UBillboardComponent;
class UArrowComponent;
class UTextRenderComponent;

/**
 * 정찰 경로의 포인트.
 */
UCLASS()
class DEMOPORTFOLIO_API APatrolPoint : public AActor
{
	GENERATED_BODY()

public:
	APatrolPoint();

	void SetOwningPath(APatrolPath* InPath);

#if WITH_EDITOR
	virtual void PostEditMove(bool bFinished) override;

	// 에디터 비주얼 갱신
	void UpdateEditorVisuals();
#endif

private:
	// 소속 정찰 경로 포인트
	UPROPERTY()
	TWeakObjectPtr<APatrolPath> OwningPath;

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBillboardComponent> EditorBillboard;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> EditorArrow;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextRenderComponent> EditorIndexText;
#endif
};
