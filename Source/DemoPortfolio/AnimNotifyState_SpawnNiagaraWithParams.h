// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_SpawnNiagaraWithParams.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "SpawnNiagaraWithParams"))
class DEMOPORTFOLIO_API UAnimNotifyState_SpawnNiagaraWithParams : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UAnimNotifyState_SpawnNiagaraWithParams();

#if WITH_EDITOR
	virtual FString GetNotifyName_Implementation() const override;
#endif
};
