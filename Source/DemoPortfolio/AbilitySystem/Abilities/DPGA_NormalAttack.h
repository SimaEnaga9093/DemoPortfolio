// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DPGameplayAbility.h"
#include "DPGA_NormalAttack.generated.h"

/**
 * 
 */
UCLASS()
class DEMOPORTFOLIO_API UDPGA_NormalAttack : public UDPGameplayAbility
{
	GENERATED_BODY()
	
public:
	UDPGA_NormalAttack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, Category = "DemoPortfolio|Attack")
	float AttackRange = 120.0f;

	UPROPERTY(EditDefaultsOnly, Category = "DemoPortfolio|Attack")
	float AttackRadius = 60.0f;

private:
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageInterrupted();
};
