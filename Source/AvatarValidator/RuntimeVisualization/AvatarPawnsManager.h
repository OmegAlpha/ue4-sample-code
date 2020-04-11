// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AvatarConfigsLoader.h"
#include "AvatarVisualizationActor.h"
#include "VisualizationPawn.h"
#include "Camera/CameraActor.h"
#include "TimerManager.h"
#include "VideoRecorder.h"
#include "AvatarsFilter.h"
#include "AvatarPawnsManager.generated.h"


class UAvatarsFilterAvatars;
class UAvatarsFilterAnimations;
class UWUI_AvatarsVisualization;

UENUM(BlueprintType)
enum class EFilterType : uint8
{
	AvatarS 	UMETA(DisplayName = "Avatars"),
	ANIMATIONS 	UMETA(DisplayName = "Animations")
};

UCLASS()
class AvatarVALIDATOR_API AAvatarPawnsManager : public AActor
{
	GENERATED_BODY()

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWUI_AvatarsVisualization> MainWidget_Class;

	UPROPERTY()
	UWUI_AvatarsVisualization* MainWidget;

	UPROPERTY()
	UVisualizationConfigDataAsset* Config;

	UPROPERTY()
	UAvatarConfigsLoader* ConfigsLoader;

	// Stores the state of how User modifies Visualization in Runtime (will be also serialized)
	UPROPERTY()
	FVisualizationSettingsState SettingsState;

	UPROPERTY()
	FTimerHandle AutoSwitchAvatar_TimerHandle;

	FTimerDelegate AutoSwitchAvatar_TimerDel;

	UPROPERTY()
	UVideoRecorder* VideoRecorder;	

	UPROPERTY()
	UAvatarsFilter* SelectedFilter;

	UPROPERTY()
	float TurnTableRatio; // from 0 to 1, set from the UI Slider

public:

	UPROPERTY()
	UAvatarsFilterAvatars* FilterAvatars;

	UPROPERTY()
	UAvatarsFilterAnimations* FilterAnimations;

	// Members and Config
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAvatarVisualizationActor> AvatarActorClass_BP;

	UPROPERTY(BlueprintReadOnly)
	TArray<AAvatarVisualizationActor*> AllAvatarsActors;

public:

	AAvatarPawnsManager();

	// Called when the game starts
	// We are using this one instead of BeginPlay and call it from the GameMode to be sure 
	// we do it in the right order
	virtual void Initialize();

	UFUNCTION()
	FORCEINLINE UVisualizationConfigDataAsset* GetConfig() const { return Config; }

	UFUNCTION(BlueprintCallable)
	UVideoRecorder* GetVideoRecorder() const { return VideoRecorder; }

	UFUNCTION()
	UWUI_AvatarsVisualization* GetMainWidget() { return MainWidget; }

	UFUNCTION()
	FVisualizationSettingsState GetCurrentSettingsState() { return SettingsState; }

	UFUNCTION()
	UAvatarsFilter* GetSelectedFilter() { return SelectedFilter; }

	virtual void Tick(float DeltaTime) override;

	// Set Auto Avatars Switch On/Off
	UFUNCTION(BlueprintCallable)
	void ToggleAutoAvatarSwitch(bool IsOn);

	UFUNCTION(BlueprintCallable)
	void SetTurnTableRatio(float Value);

	UFUNCTION(BlueprintCallable)
	void SelectFilter(EFilterType FilterType);

	// Selects the first element in the list of Primary Filter Elements
	UFUNCTION(BlueprintCallable)
	void SelectFirstPrimary();

	UFUNCTION(BlueprintCallable)
	void SelectNextPrimary();

	UFUNCTION(BlueprintCallable)
	void SelectNext_Avatar();

	UFUNCTION(BlueprintCallable)
	void SelectPrevious_Avatar();

	UFUNCTION(BlueprintCallable)
	void SelectNext_Animation();

	UFUNCTION(BlueprintCallable)
	void SelectPrevious_Animation();

	UFUNCTION(BlueprintCallable)
	FString GetSelectedAvatarName() const { return SelectedFilter->GetSelectedAvatarName(); }

	UFUNCTION(BlueprintCallable)
	FString GetSelectedAnimationName() const { return SelectedFilter->GetSelectedAnimationName(); }
};

