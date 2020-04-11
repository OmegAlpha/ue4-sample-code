// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AvatarConfig.h"
#include "AvatarConfigsLoader.generated.h"

/**
 * 
 */
UCLASS()
class SHAREDCLASSES_API UAvatarConfigsLoader : public UObject
{
	GENERATED_BODY()

public: 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	TArray<FAvatarConfig> AllConfigs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	TMap<FString, FAvatarConfig> AllConfigsByAvatarId;

public:

	UFUNCTION()
	void LoadAll();

	UFUNCTION()
	TArray<FAvatarConfig> GetConfigsForTheseIds(TArray<FString> AvatarIds) const;	
};
