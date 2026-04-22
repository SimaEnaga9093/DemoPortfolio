// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GameplayAbilitySpecHandle.h"

#include "DPCharacterBase.generated.h"

class UDPAbilitySystemComponent;
class UDPAttributeSet;
class UDPGameplayAbility;

UCLASS()
class DEMOPORTFOLIO_API ADPCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADPCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent.Get(); }

	FGameplayAbilitySpecHandle GetAbilitySpecHandle(FString Key) { return *AbilitySpecHandles.Find(Key); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DemoPortfolio", meta = (AllowPrivateAccess = "true"))
	TMap<FString, TSubclassOf<UDPGameplayAbility>> Abilities;

private:
	void InitAbilites();

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UDPAttributeSet> AttributeSet;

	UPROPERTY()
	TMap<FString, FGameplayAbilitySpecHandle> AbilitySpecHandles;
};
