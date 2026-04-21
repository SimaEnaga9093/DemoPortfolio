// Fill out your copyright notice in the Description page of Project Settings.


#include "DPCharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DPAbilitySystemComponent.h"
#include "DPAttributeSet.h"
#include "DPGameplayAbility.h"

// Sets default values
ADPCharacterBase::ADPCharacterBase()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	AbilitySystemComponent = CreateDefaultSubobject<UDPAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UDPAttributeSet>(TEXT("AttributeSet"));

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

// Called when the game starts or when spawned
void ADPCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADPCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADPCharacterBase::GrantAbility(TSubclassOf<UDPGameplayAbility> AbilityClass)
{
	if (AbilitySystemComponent && AbilityClass)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass));
	}
}
