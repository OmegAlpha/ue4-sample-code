// Fill out your copyright notice in the Description page of Project Settings.


#include "WUI_PanelPrevNextSelectors.h"
#include "AvatarPawnsManager.h"
#include "Button.h"

void UWUI_PanelPrevNextSelectors::Init(AAvatarPawnsManager* Manager)
{
	AvatarsManager = Manager;

	Button_Avatar_Prev->OnClicked.AddDynamic(this, &UWUI_PanelPrevNextSelectors::OnClicked_AvatarPrev);
	Button_Avatar_Next->OnClicked.AddDynamic(this, &UWUI_PanelPrevNextSelectors::OnClicked_AvatarNext);
	Button_Anim_Prev->OnClicked.AddDynamic(this, &UWUI_PanelPrevNextSelectors::OnClicked_AnimPrev);
	Button_Anim_Next->OnClicked.AddDynamic(this, &UWUI_PanelPrevNextSelectors::OnClicked_AnimNext);
}

void UWUI_PanelPrevNextSelectors::OnClicked_AvatarPrev()
{
	AvatarsManager->SelectPrevious_Avatar();
}

void UWUI_PanelPrevNextSelectors::OnClicked_AvatarNext()
{
	AvatarsManager->SelectNext_Avatar();
}

void UWUI_PanelPrevNextSelectors::OnClicked_AnimPrev()
{
	AvatarsManager->SelectPrevious_Animation();
}

void UWUI_PanelPrevNextSelectors::OnClicked_AnimNext()
{
	AvatarsManager->SelectNext_Animation();
}
