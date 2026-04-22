// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DPGameplayAbility.generated.h"

class UAnimMontage;

/**
 * 
 */
UCLASS()
class DEMOPORTFOLIO_API UDPGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	void EndAbilityOnMontage(bool bWasCancelled);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "DemoPortfolio")
	TArray<TObjectPtr<UAnimMontage>> Montages;

private:
	FGameplayAbilitySpecHandle CachedHandle;
	const FGameplayAbilityActorInfo* CachedActorInfo = nullptr;
	FGameplayAbilityActivationInfo CachedActivationInfo;
};
