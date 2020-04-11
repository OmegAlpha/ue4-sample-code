// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MovieSceneCapture/Public/MovieSceneCapture.h"
#include "VideoRecorder.generated.h"

class AAvatarPawnsManager;

// Aux class to avoid Deletion of MovieCapture when it finishes (which makes Engine Crash). 
class TAvoidDeletion
{
public:
	void operator()(void*) {}
};

USTRUCT()
struct FVisualizationSettingsState
{
	GENERATED_BODY()

	UPROPERTY()
	float TurnTableSpeed;

	UPROPERTY()
	bool bAutoSwitchAvatars;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVideoRecordFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVideoRecordStarted);

UCLASS()
class AvatarVALIDATOR_API UVideoRecorder : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnVideoRecordFinished OnVideoRecordFinished;

	UPROPERTY(BlueprintAssignable)
	FOnVideoRecordStarted OnVideoRecordStarted;

private:

	FTimerDelegate VideoRecordingTimer_TimerDel;

	// This will tick each time a SubItem was recorded
	// When all the SubItems were recorded, the Main Item finished 
	// For instance, if you are recording a Avatar, this will tick each time one of the animations finishes recording
	// and then the whole Avatar video will stop capturing
	UPROPERTY()
	FTimerHandle VideoRecordingTimer_TimerHandle;

	UPROPERTY()
	AAvatarPawnsManager* AvatarsManager;

	UPROPERTY()
	FString OutputPath;

	UPROPERTY()
	FString CurrentVideoName;

	UPROPERTY()
	bool bIsRecordingAll;

	UPROPERTY()
	int TotalItems;

	UPROPERTY()
	int TotalSubItems;

	// true if recording animations, false if recording Avatars
	// TODO: refactor this to some Enum type instead?
	UPROPERTY()
	bool bRecordingAnimations;

	UPROPERTY()
	UMovieSceneCapture* MovieSceneCapture;

	UPROPERTY()
	FTimerHandle Delay_TimerHandle;

	FTimerDelegate Delay_TimerDel;

	// Stores a temporal saved state that will be restored after Video finishes recording
	FVisualizationSettingsState SettingsStateVideoTemp;

public:

	void Initialize(AAvatarPawnsManager* AvatarsManagerPtr);

	UFUNCTION()
	void StartCapturing();

	UFUNCTION()
	void StopCapturing();

	UFUNCTION()
	void CaptureAfterDelay();

	UFUNCTION(BlueprintCallable)
	void VideoRecordAll();

	UFUNCTION()
	void OnRecordSubItemFinished();

	UFUNCTION(BlueprintCallable)
	void RecordCurrentMainItem();

private:

	// if recording animations, this will record the current selected Avatar for that animation.
	// if recording Avatars, this will record the current selected animation for that Avatar
	UFUNCTION()
	void RecordCurrentSubItem();

	UFUNCTION()
	float GetCurrentSubItemDuration() const;

	UFUNCTION()
	bool OpenSelectFolderDialog();
};
