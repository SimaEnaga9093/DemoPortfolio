// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "AnimNotifyState_HitTrace.generated.h"

/**
 * 공격 판정 레코딩 노티파이
 */
UCLASS(meta = (DisplayName = "HitTrace"))
class DEMOPORTFOLIO_API UAnimNotifyState_HitTrace : public UAnimNotifyState
{
    GENERATED_BODY()

public:
    UAnimNotifyState_HitTrace();

    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
    virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

#if WITH_EDITOR
    virtual FString GetNotifyName_Implementation() const override;
#endif

protected:
    // 어태치할 본 이름. 없으면 안 됨.
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName AttachBoneName = TEXT("weapon_r_socket");

    // 박스의 오프셋
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FVector LocalOffset = FVector::ZeroVector;

    // 박스의 절반 사이즈
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "1.0"))
    FVector BoxHalfExtent = FVector(60.f, 25.f, 25.f);

    // 타격 판정을 할 채널
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Pawn;

    // 타격 판정 시 전송할 GameplayTag
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag HitEventTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay)
    bool bDrawDebug = true;

private:
    // 액터별로 공격 판정 인스턴싱
    struct FAttackInfo
    {
        TSet<TWeakObjectPtr<AActor>> HitActors;
        FVector PreviousLocation = FVector::ZeroVector;
    };
    TMap<TWeakObjectPtr<AActor>, FAttackInfo> InstancedAttackInfos;

    // 위치, 회전 계산
    FVector CalcWorldLocation(const USkeletalMeshComponent* MeshComp) const;
    FQuat CalcWorldRotation(const USkeletalMeshComponent* MeshComp) const;

    void SendAttackEventByTrace(USkeletalMeshComponent* MeshComp, FAttackInfo& AttackInfo, const FVector& Start, const FVector& End);
};
