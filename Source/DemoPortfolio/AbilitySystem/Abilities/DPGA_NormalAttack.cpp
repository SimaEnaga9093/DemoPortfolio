// Fill out your copyright notice in the Description page of Project Settings.


#include "DPGA_NormalAttack.h"

#include "GameFramework/Character.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UDPGA_NormalAttack::UDPGA_NormalAttack()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UDPGA_NormalAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
    if (!Character)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    if (APlayerController* PC = Cast<APlayerController>(Character->GetController()))
    {
        FHitResult Hit;
        if (PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit))
        {
            FVector Direction = Hit.Location - Character->GetActorLocation();
            Direction.Z = 0.0f;

            if (!Direction.IsNearlyZero())
            {
                Character->SetActorRotation(Direction.Rotation());
            }
        }
    }

    UAbilityTask_PlayMontageAndWait* Task =
        UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, Montages[0].Get());

    Task->OnCompleted.AddDynamic(this, &UDPGA_NormalAttack::OnMontageCompleted);
    Task->OnBlendOut.AddDynamic(this, &UDPGA_NormalAttack::OnMontageCompleted);
    Task->OnInterrupted.AddDynamic(this, &UDPGA_NormalAttack::OnMontageInterrupted);
    Task->OnCancelled.AddDynamic(this, &UDPGA_NormalAttack::OnMontageInterrupted);

    Task->ReadyForActivation();

    FVector Start = Character->GetActorLocation();
    FVector End = Start + Character->GetActorForwardVector() * AttackRange;

    TArray<FHitResult> Hits;
    GetWorld()->SweepMultiByChannel(Hits, Start, End, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(AttackRadius));

    for (const FHitResult& Hit : Hits)
    {
        if (Hit.GetActor() && Hit.GetActor() != Character)
        {
            UE_LOG(LogTemp, Log, TEXT("Hit: %s"), *Hit.GetActor()->GetName());
        }
    }

}

void UDPGA_NormalAttack::OnMontageCompleted()
{
    EndAbilityOnMontage(false);
}

void UDPGA_NormalAttack::OnMontageInterrupted()
{
    EndAbilityOnMontage(true);
}
