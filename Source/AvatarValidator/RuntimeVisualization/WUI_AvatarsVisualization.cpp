// Fill out your copyright notice in the Description page of Project Settings.


#include "WUI_AvatarsVisualization.h"
#include "WUI_FilterAvatarsPanel.h"
#include "WUI_FilterAnimationsPanel.h"
#include "WUI_CameraControlsPanel.h"
#include "WUI_PanelPrevNextSelectors.h"
#include "WUI_VideoRecordControlPanel.h"


void UWUI_AvatarsVisualization::Init(AAvatarPawnsManager* Manager)
{
	AvatarsManager = Manager;

	Panel_FilterAvatars->Init(AvatarsManager->FilterAvatars);
	Panel_FilterAnimations->Init(AvatarsManager->FilterAnimations);

	Button_Filter_Avatars->OnClicked.AddDynamic(this, &UWUI_AvatarsVisualization::OnClicked_ShowAvatarsFilter);
	Button_Filter_Animations->OnClicked.AddDynamic(this, &UWUI_AvatarsVisualization::OnClicked_ShowAnimationsFilter);

	WBP_CameraControlsPanel->Init(Manager);
	WBP_PanelPrevNextSelectors->Init(Manager);

	OnClicked_ShowAvatarsFilter();

	WBP_VideoRecordControlPanel->Init(Manager);

	// 	AvatarsManager->OnVideoRecordFinished.AddDynamic(this, &UWUI_AvatarsVisualization::OnVideoRecordFinished);
}

void UWUI_AvatarsVisualization::OnVideoRecordFinished()
{
	this->SetVisibility(ESlateVisibility::Visible);
}

void UWUI_AvatarsVisualization::OnClicked_ShowAnimationsFilter()
{
	UE_LOG(LogTemp, Verbose, TEXT("SWAP TO ANIMATIONS"));
	Panel_FilterAvatars->SetVisibility(ESlateVisibility::Hidden);
	Panel_FilterAnimations->SetVisibility(ESlateVisibility::Visible);

	AvatarsManager->SelectFilter(EFilterType::ANIMATIONS);
}

void UWUI_AvatarsVisualization::OnClicked_ShowAvatarsFilter()
{
	UE_LOG(LogTemp, Verbose, TEXT("SWAP TO AvatarS"));
	Panel_FilterAvatars->SetVisibility(ESlateVisibility::Visible);
	Panel_FilterAnimations->SetVisibility(ESlateVisibility::Hidden);

	AvatarsManager->SelectFilter(EFilterType::AvatarS);
}
