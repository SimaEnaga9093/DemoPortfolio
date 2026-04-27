// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_SpawnNiagaraWithParams.h"

UAnimNotifyState_SpawnNiagaraWithParams::UAnimNotifyState_SpawnNiagaraWithParams()
{
#if WITH_EDITORONLY_DATA
    NotifyColor = FColor::Cyan;
#endif
}

#if WITH_EDITOR
FString UAnimNotifyState_SpawnNiagaraWithParams::GetNotifyName_Implementation() const
{
    return FString::Printf(TEXT("SpawnNiagara [Sword_Slash]"));
}
#endif
