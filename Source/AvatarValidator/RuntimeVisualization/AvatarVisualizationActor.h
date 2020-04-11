// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AvatarConfig.h"
#include "Components/SceneCaptureComponent.h"
#include "AvatarVisualizationActor.generated.h"

UCLASS()
class AvatarVALIDATOR_API AAvatarVisualizationActor : public APawn
{
	GENERATED_BODY()

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfigUpdated, FAvatarConfig, NewConfig);

	UPROPERTY(BlueprintAssignable)
	FOnConfigUpdated OnConfigUpdated;

	UPROPERTY(BlueprintReadOnly)
	bool IsFilteredIn;

protected:

	UPROPERTY()
	class UVisualizationConfigDataAsset* GlobalConfig;

	UPROPERTY(Category = "Visualization", VisibleAnywhere, BlueprintReadOnly)
	USceneCaptureComponent* RootScene;

	UPROPERTY(Category = "Visualization", VisibleAnywhere, BlueprintReadOnly)
	FAvatarConfig Config;

	UPROPERTY(Category = "Visualization", VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* MasterMaterial_Avatar;

	UPROPERTY()
	UMaterialInstanceDynamic* MaterialInstance;

	UPROPERTY()
	float TurnTableSpeedFactor;

public:	
	
	AAvatarVisualizationActor();

	void SetConfig(FAvatarConfig NewConfig);

	FORCEINLINE FAvatarConfig GetConfig() const { return Config; }

	virtual void Tick(float DeltaTime) override;

	void SetTurnTableSpeedFactor(float Speed);

	void TryPlayAnimation(class UAnimationAsset* Animation);

protected:

	virtual void BeginPlay() override;

};
