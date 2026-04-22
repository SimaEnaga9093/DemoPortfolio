// Fill out your copyright notice in the Description page of Project Settings.


#include "DPGameplayAbility.h"
#include "Animation/AnimMontage.h"

void UDPGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    CachedHandle = Handle;
    CachedActorInfo = ActorInfo;
    CachedActivationInfo = ActivationInfo;

    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UDPGameplayAbility::EndAbilityOnMontage(bool bWasCancelled)
{
    EndAbility(CachedHandle, CachedActorInfo, CachedActivationInfo, true, bWasCancelled);
}