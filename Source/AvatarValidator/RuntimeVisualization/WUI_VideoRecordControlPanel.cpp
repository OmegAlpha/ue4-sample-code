// Fill out your copyright notice in the Description page of Project Settings.


#include "WUI_VideoRecordControlPanel.h"
#include "AvatarPawnsManager.h"
#include "Button.h"
#include "VideoRecorder.h"
#include "WUI_AvatarsVisualization.h"

void UWUI_VideoRecordControlPanel::Init(AAvatarPawnsManager* AvatarsManagerPtr)
{
	AvatarsManager = AvatarsManagerPtr;
	
	UVideoRecorder* VideoRecorder = AvatarsManager->GetVideoRecorder();

	Btn_RecordAll->OnClicked.AddDynamic(VideoRecorder, &UVideoRecorder::VideoRecordAll);
	Btn_RecordThis->OnClicked.AddDynamic(VideoRecorder, &UVideoRecorder::RecordCurrentMainItem);

	VideoRecorder->OnVideoRecordStarted.AddDynamic(this, &UWUI_VideoRecordControlPanel::OnVideoRecordStarted);
	VideoRecorder->OnVideoRecordFinished.AddDynamic(this, &UWUI_VideoRecordControlPanel::OnVideoRecordFinished);
}

void UWUI_VideoRecordControlPanel::OnVideoRecordStarted()
{
	AvatarsManager->GetMainWidget()->SetVisibility(ESlateVisibility::Hidden);
}

void UWUI_VideoRecordControlPanel::OnVideoRecordFinished()
{
	AvatarsManager->GetMainWidget()->SetVisibility(ESlateVisibility::Visible);
}
