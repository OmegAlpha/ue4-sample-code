// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "VisualizationConfigDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class AvatarVALIDATOR_API UVisualizationConfigDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FString> FilteredAvatars;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visualization")
	float TurnTableMaxSpeed = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visualization")
	float AutoSwitchAvatarDelay = 3.f;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cameras")
	float AutoFitFreeCameraToLastCamera = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cameras")
	float FreeCamera_MovementSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cameras")
	float FreeCamera_RotationSpeed = 350.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VideoRecording")
	FString SaveVideosPath = FPaths::ProjectContentDir().Replace(TEXT("Content"), TEXT("AvatarVideos"));

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VideoRecording")
	float MinAnimationRecordingTime = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VideoRecording")
	float bUseTurnTableOnVideo = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VideoRecording")
	float TurnTableSpeedOnVideo = 1.5f;
};
