// Fill out your copyright notice in the Description page of Project Settings.


#include "WUI_AnimationFilterRow.h"
#include "TextBlock.h"
#include "Animation/AnimSequence.h"
#include "Text.h"

void UWUI_AnimationFilterRow::SetSelected(bool Selected)
{
	CheckBox_Selected->SetCheckedState(Selected ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	OnCheckBoxChanged(Selected);
}

void UWUI_AnimationFilterRow::Init(UOneAnimationData* AnimData)
{
	AnimationData = AnimData;

	Text_AnimationName->SetText(FText::FromString(AnimationData->Animation->GetName()));

	CheckBox_Selected->OnCheckStateChanged.AddDynamic(this, &UWUI_AnimationFilterRow::OnCheckBoxChanged);
	Btn_ViewThis->OnClicked.AddDynamic(this, &UWUI_AnimationFilterRow::ViewThis);

	SetSelected(true);
}

void UWUI_AnimationFilterRow::OnCheckBoxChanged(bool Value)
{
	Btn_ViewThis->SetVisibility(Value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	Img_BackgroundSelected->SetVisibility(Value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	AnimationData->bIsFilteredIn = Value;

	OnCheckedChanged.Broadcast();
}

void UWUI_AnimationFilterRow::ViewThis()
{
	OnClicked.Broadcast(AnimationData);
}
