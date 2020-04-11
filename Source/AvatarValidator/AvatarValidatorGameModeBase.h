// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AvatarValidatorGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class AvatarVALIDATOR_API AAvatarValidatorGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	AAvatarValidatorGameModeBase();

	void BeginPlay() override;
};
