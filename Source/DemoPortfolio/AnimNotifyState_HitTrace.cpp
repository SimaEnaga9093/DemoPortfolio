// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_HitTrace.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameplayTagContainer.h"

UAnimNotifyState_HitTrace::UAnimNotifyState_HitTrace()
{
#if WITH_EDITORONLY_DATA
    NotifyColor = FColor(220, 70, 70);
#endif
}

void UAnimNotifyState_HitTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

    if (!IsValid(MeshComp))
    {
        return;
    }

    // 타격 정보 인스턴싱
    AActor* OwnerActor = MeshComp->GetOwner();
    if (OwnerActor)
    {
        FAttackInfo& AttackInfo = InstancedAttackInfos.FindOrAdd(OwnerActor);
        AttackInfo.HitActors.Reset();
        AttackInfo.PreviousLocation = CalcWorldLocation(MeshComp);
    }
}

void UAnimNotifyState_HitTrace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

    if (!IsValid(MeshComp))
    {
        return;
    }

    AActor* OwnerActor = MeshComp->GetOwner();
    if (OwnerActor)
    {
        FAttackInfo* AttackInfo = InstancedAttackInfos.Find(OwnerActor);
        if (!AttackInfo)
        {
            AttackInfo = &InstancedAttackInfos.FindOrAdd(OwnerActor);
            AttackInfo->PreviousLocation = CalcWorldLocation(MeshComp);
            return;
        }

        const FVector CurrentLocation = CalcWorldLocation(MeshComp);
        const FVector StartLocation = AttackInfo->PreviousLocation;

        // 타격 판정
        SendAttackEventByTrace(MeshComp, *AttackInfo, StartLocation, CurrentLocation);

        AttackInfo->PreviousLocation = CurrentLocation;
    }
}

void UAnimNotifyState_HitTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyEnd(MeshComp, Animation, EventReference);

    AActor* OwnerActor = MeshComp->GetOwner();
    if (OwnerActor)
    {
        // 인스턴싱된 데이터 삭제
        InstancedAttackInfos.Remove(OwnerActor);
    }
}

FVector UAnimNotifyState_HitTrace::CalcWorldLocation(const USkeletalMeshComponent* MeshComp) const
{
    const FTransform BoneTransform = MeshComp->GetSocketTransform(AttachBoneName, RTS_World);
    return BoneTransform.TransformPosition(LocalOffset);
}

FQuat UAnimNotifyState_HitTrace::CalcWorldRotation(const USkeletalMeshComponent* MeshComp) const
{
    return MeshComp->GetSocketTransform(AttachBoneName, RTS_World).GetRotation();
}

void UAnimNotifyState_HitTrace::SendAttackEventByTrace(USkeletalMeshComponent* MeshComp, FAttackInfo& AttackInfo, const FVector& Start, const FVector& End)
{
    UWorld* World = MeshComp->GetWorld();
    const FQuat BoxRotation = CalcWorldRotation(MeshComp);

#if ENABLE_DRAW_DEBUG
    // 궤적 그리기
    if (bDrawDebug)
    {
        DrawDebugBox(World, (Start + End) * 0.5f, BoxHalfExtent, BoxRotation, FColor::Red, false, 0.5f, 0, 0.5f);
        DrawDebugLine(World, Start, End, FColor::Yellow, false, 0.5f, 0, 0.5f);
    }
#endif

    // 유효한 월드, 액터가 아니라면 타격 판정하지 않음. (에디터에서는 무시함)
    AActor* OwnerActor = MeshComp->GetOwner();
    if (!World || !World->IsGameWorld() || !OwnerActor)
    {
        return;
    }

    // Trace 옵션 만들기 (최적화 확인용 SCENE_QUERY_STAT)
    const FCollisionShape CollisionShape = FCollisionShape::MakeBox(BoxHalfExtent);
    FCollisionQueryParams Params(SCENE_QUERY_STAT(HitTraceNotify), false, OwnerActor);

    TArray<FHitResult> Hits;
    World->SweepMultiByChannel(Hits, Start, End, BoxRotation, TraceChannel, CollisionShape, Params);

    for (const FHitResult& Hit : Hits)
    {
        AActor* HitActor = Hit.GetActor();
        if (!HitActor || HitActor == OwnerActor)
        {
            continue;
        }

        if (AttackInfo.HitActors.Contains(HitActor))
        {
            continue;
        }
        AttackInfo.HitActors.Add(HitActor);

        // Hit Data 만들기
        FGameplayEventData EventData;
        EventData.EventTag = HitEventTag;
        EventData.Instigator = OwnerActor;
        EventData.Target = HitActor;
        EventData.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(Hit);

        // EventTag로 타격 이벤트 전송
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitEventTag, EventData);
    }
}

#if WITH_EDITOR
FString UAnimNotifyState_HitTrace::GetNotifyName_Implementation() const
{
    // 타임라인에 표시하여 어떤 본 판정인지 식별
    return FString::Printf(TEXT("HitTrace [%s]"), *AttachBoneName.ToString());
}
#endif
