// Fill out your copyright notice in the Description page of Project Settings.

#include "VideoRecorder.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "GameFramework/GameUserSettings.h"
#include "UnrealClient.h"
#include "SceneViewport.h"
#include "LevelEditor.h"
#include "ILevelViewport.h"
#include "PlatformFilemanager.h"
#include "AvatarPawnsManager.h"
#include "VisualizationConfigDataAsset.h"
#include "AvatarsFilterAnimations.h"
#include "Animation/AnimSequence.h"
#include "RotationActor.h"
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"

void UVideoRecorder::Initialize(AAvatarPawnsManager* AvatarsManagerPtr)
{
	AvatarsManager = AvatarsManagerPtr;
	OutputPath = AvatarsManager->GetConfig()->SaveVideosPath;

	MovieSceneCapture = NewObject< UMovieSceneCapture>();	

	Delay_TimerDel.BindUFunction(this, FName("CaptureAfterDelay"));
	VideoRecordingTimer_TimerDel.BindUFunction(this, FName("OnRecordSubItemFinished"));
}

void UVideoRecorder::StartCapturing()
{
	FLevelEditorModule& LevelEditor = FModuleManager::GetModuleChecked<FLevelEditorModule>("LevelEditor");
	ILevelViewport* LevelViewport = LevelEditor.GetFirstActiveViewport().Get();
	LevelViewport->MakeImmersive(true, false);
	
	// We apply a delay between we make the LevelVieport Immersive and we start recording, so we don't record the lag
	AvatarsManager->GetWorld()->GetTimerManager().SetTimer(
		Delay_TimerHandle,
		Delay_TimerDel,
		0.5f,
		false,
		0.5f);
}

void UVideoRecorder::StopCapturing()
{
	MovieSceneCapture->Finalize();	

	FString AbsoluteFilePath = OutputPath / CurrentVideoName + ".avi";
	FString AbsoluteFilePath_Renamed = OutputPath / CurrentVideoName + "_" + FDateTime::Now().ToString() + ".avi";

	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*AbsoluteFilePath))
	{
		if (!FPlatformFileManager::Get().GetPlatformFile().MoveFile(*AbsoluteFilePath_Renamed, *AbsoluteFilePath))
		{
			UE_LOG(LogTemp, Error, TEXT("Video could not be renamed with TimeStamp for: %s"), *CurrentVideoName)
		}
	}	

	UAvatarsFilter* Filter = AvatarsManager->GetSelectedFilter();

	if(bIsRecordingAll && Filter->GetPrimarySelectedIndex() < Filter->GetPrimaryElementsQuantity() - 1)
	{
		Filter->SelectNextPrimary();
		Filter->SelectFirstSecondary();
		RecordCurrentMainItem();
	}
	else
	{
		AvatarsManager->ToggleAutoAvatarSwitch(SettingsStateVideoTemp.bAutoSwitchAvatars);
		AvatarsManager->GetRotationManager()->Reset();
		bIsRecordingAll = false;

		FLevelEditorModule& LevelEditor = FModuleManager::GetModuleChecked<FLevelEditorModule>("LevelEditor");
		ILevelViewport* LevelViewport = LevelEditor.GetFirstActiveViewport().Get();
		LevelViewport->MakeImmersive(false, false);

		OnVideoRecordFinished.Broadcast();
	}
}

void UVideoRecorder::CaptureAfterDelay()
{
	AvatarsManager->GetWorld()->GetTimerManager().ClearTimer(Delay_TimerHandle);

	FDirectoryPath Path;
	Path.Path = OutputPath;

	FMovieSceneCaptureSettings SceneCapSettings;
	SceneCapSettings.OutputDirectory = Path;

	SceneCapSettings.OutputFormat = CurrentVideoName;
	SceneCapSettings.bOverwriteExisting = true;
	SceneCapSettings.Resolution = FCaptureResolution(1920, 1080);
	SceneCapSettings.bCinematicMode = true;
	SceneCapSettings.bCinematicEngineScalability = true;

	MovieSceneCapture->Settings = SceneCapSettings;

	UGameViewportClient* ViewportClient = GEngine->GameViewport;
	FSceneViewport* ViewportPtr = ViewportClient->GetGameViewport();

	TSharedPtr<FSceneViewport> Viewport = MakeShareable(ViewportPtr, TAvoidDeletion());
	MovieSceneCapture->Initialize(Viewport);
	MovieSceneCapture->StartCapture();	

	RecordCurrentSubItem();
}

void UVideoRecorder::VideoRecordAll()
{
	UE_LOG(LogTemp, Log, TEXT("Start Recording All"));

	// This Let user select Target Export folder, but cancels the recording if no folder was selected
	if (!OpenSelectFolderDialog())
	{
		return;
	}

	bIsRecordingAll = true;
	
	AvatarsManager->GetSelectedFilter()->SelectFirstPrimary();
	AvatarsManager->GetSelectedFilter()->SelectFirstSecondary();	

	RecordCurrentMainItem();
}


void UVideoRecorder::RecordCurrentMainItem()
{
	// Only open dialog if we are recording a single Item
	if (!bIsRecordingAll)
	{
		// This Let user select Target Export folder, but cancels the recording if no folder was selected
		if (!OpenSelectFolderDialog())
		{
			return;
		}
	}

	OnVideoRecordStarted.Broadcast();

	SettingsStateVideoTemp = FVisualizationSettingsState(AvatarsManager->GetCurrentSettingsState());
	
	bRecordingAnimations = Cast<UAvatarsFilterAnimations>(AvatarsManager->GetSelectedFilter()) != nullptr;
	
	AvatarsManager->GetRotationManager()->Reset();
	AvatarsManager->GetRotationManager()->StartRotating();
	AvatarsManager->ToggleAutoAvatarSwitch(false);

	if (bRecordingAnimations)
	{
		CurrentVideoName = AvatarsManager->GetSelectedFilter()->GetSelectedAnimationName();		
		UE_LOG(LogTemp, Log, TEXT("Start Recording Animation: %s"), *CurrentVideoName);
	}
	else
	{
		CurrentVideoName = AvatarsManager->GetSelectedFilter()->GetSelectedAvatarName();
		UE_LOG(LogTemp, Log, TEXT("Start Recording Avatar: %s"), *CurrentVideoName);
	}	

	StartCapturing();
}

void UVideoRecorder::RecordCurrentSubItem()
{
	AvatarsManager->GetWorld()->GetTimerManager().SetTimer(
		VideoRecordingTimer_TimerHandle,
		VideoRecordingTimer_TimerDel,
		GetCurrentSubItemDuration(),
		false,
		GetCurrentSubItemDuration()
	);
}

float UVideoRecorder::GetCurrentSubItemDuration() const
{
	float Duration = 0;

	UOneAnimationData* AnimationData = AvatarsManager->GetSelectedFilter()->GetSelectedAnimationData();

	if (AnimationData != nullptr)
	{
		UAnimSequence* AnimSequence = Cast<UAnimSequence>(AnimationData);

		if (AnimSequence != nullptr)
		{
			Duration = AnimSequence->SequenceLength;
		}
	}

	if (Duration < AvatarsManager->GetConfig()->MinAnimationRecordingTime)
	{
		Duration = AvatarsManager->GetConfig()->MinAnimationRecordingTime;
	}

	return Duration;
}

bool UVideoRecorder::OpenSelectFolderDialog()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

	if (DesktopPlatform)
	{
		FString SelectionPath = AvatarsManager->GetConfig()->SaveVideosPath;

		bool FolderWasSelected = DesktopPlatform->OpenDirectoryDialog(nullptr, TEXT("Select Video Export Folder"), SelectionPath, OutputPath);

		if(FolderWasSelected)
		{
			AvatarsManager->GetConfig()->SaveVideosPath = OutputPath;
			AvatarsManager->GetConfig()->SaveConfig();
		}

		return FolderWasSelected;
	}

	return false;
}

void UVideoRecorder::OnRecordSubItemFinished()
{
	AvatarsManager->GetWorld()->GetTimerManager().ClearTimer(VideoRecordingTimer_TimerHandle);

	UAvatarsFilter* Filter = AvatarsManager->GetSelectedFilter();

	bool bFinishedAllSubItems = Filter->GetSecondarySelectedIndex() >= Filter->GetSecondaryElementsQuantity() - 1;

	if (bFinishedAllSubItems)
	{
		StopCapturing();
	}
	else
	{
		Filter->SelectNextSecondary();
		RecordCurrentSubItem();
	}
}