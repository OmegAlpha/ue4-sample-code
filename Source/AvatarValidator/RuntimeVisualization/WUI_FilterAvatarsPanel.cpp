// Fill out your copyright notice in the Description page of Project Settings.


#include "WUI_FilterAvatarsPanel.h"
#include "ConstructorHelpers.h"
#include "WUI_AvatarFilterRow.h"
#include "VerticalBox.h"
#include "AvatarVisualizationActor.h"
#include "AvatarsFilterAvatars.h"

UWUI_FilterAvatarsPanel::UWUI_FilterAvatarsPanel(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UWUI_AvatarFilterRow>
		AvatarRowClass(TEXT("WidgetBlueprint'/Game/Validator/UI/WBP_AvatarListRow'"));

	AvatarFilterRow_Class = AvatarRowClass.Class;

	if (AvatarFilterRow_Class == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("<AvatarFilterRow_Class> not assigned in <UWUI_FilterAvatarsPanel> Constructor. Probably the blueprint was renamed?"));
		return;
	}		
}

void UWUI_FilterAvatarsPanel::Init(UAvatarsFilterAvatars* FilterLogic)
{
	FilterAvatars = FilterLogic;

	for (AAvatarVisualizationActor* AvatarActor : FilterAvatars->AllAvatars )
	{
		UWUI_AvatarFilterRow* NewFilterRow = Cast< UWUI_AvatarFilterRow>(CreateWidget(this, AvatarFilterRow_Class));
		NewFilterRow->Init(AvatarActor);
		VB_FilterRows->AddChild(NewFilterRow);

		NewFilterRow->OnClickedViewThis.AddDynamic(FilterAvatars, &UAvatarsFilterAvatars::SelectAvatar);
		NewFilterRow->OnCheckedChanged.AddDynamic(FilterAvatars, &UAvatarsFilterAvatars::ApplyFilterPrimary);

		AllAvatarFilterRows.Add(NewFilterRow);
	}
}
