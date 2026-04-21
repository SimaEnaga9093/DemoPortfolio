// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "DPAbilitySystemComponent.h"
#include "DPAttributeSet.h"

#include "DPCharacterBase.generated.h"

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

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

private:
	UPROPERTY()
	TObjectPtr<UDPAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UDPAttributeSet> AttributeSet;
};
