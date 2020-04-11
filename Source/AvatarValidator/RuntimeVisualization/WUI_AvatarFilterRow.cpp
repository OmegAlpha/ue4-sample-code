// Fill out your copyright notice in the Description page of Project Settings.


#include "WUI_AvatarFilterRow.h"
#include "TextBlock.h"
#include "AvatarVisualizationActor.h"
#include "AvatarPawnsManager.h"

void UWUI_AvatarFilterRow::SetSelected(bool Selected)
{
	CheckBox_Selected->SetCheckedState(Selected ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	OnCheckBoxChanged(Selected);
}

void UWUI_AvatarFilterRow::Init(AAvatarVisualizationActor* Avatar)
{
	AvatarActor = Avatar;

	Text_AvatarName->SetText( FText::FromString(Avatar->GetConfig().Name));

	CheckBox_Selected->OnCheckStateChanged.AddDynamic(this, &UWUI_AvatarFilterRow::OnCheckBoxChanged);
	Btn_ViewThis->OnClicked.AddDynamic(this, &UWUI_AvatarFilterRow::ViewThis);

	SetSelected(Avatar->IsFilteredIn);	
}

void UWUI_AvatarFilterRow::OnCheckBoxChanged(bool Value)
{
	Btn_ViewThis->SetVisibility(Value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	Img_BackgroundSelected->SetVisibility(Value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	AvatarActor->IsFilteredIn = Value;
	
	UE_LOG(LogTemp, Log, TEXT("Toggled Filter Avatar: %s -> %s"), *AvatarActor->GetConfig().Name, (Value) ? TEXT("true"): TEXT("false"))

	OnCheckedChanged.Broadcast();
}

void UWUI_AvatarFilterRow::ViewThis()
{
	UE_LOG(LogTemp, Log, TEXT("Clicked View Avatar: %s"), *AvatarActor->GetConfig().Name)
	OnClickedViewThis.Broadcast(AvatarActor);
}
