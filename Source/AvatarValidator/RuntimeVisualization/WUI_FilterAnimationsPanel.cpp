// Fill out your copyright notice in the Description page of Project Settings.


#include "WUI_FilterAnimationsPanel.h"
#include "WUI_AnimationFilterRow.h"
#include "PanelWidget.h"
#include "AvatarsFilterAnimations.h"
#include "VerticalBox.h"
#include "ComboBoxString.h"

UWUI_FilterAnimationsPanel::UWUI_FilterAnimationsPanel(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UWUI_AnimationFilterRow>
		AnimationRowClass(TEXT("WidgetBlueprint'/Game/Validator/UI/WBP_AnimationListRow'"));

	AnimationFilterRow_Class = AnimationRowClass.Class;

	if (AnimationFilterRow_Class == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("<AnimationFilterRow_Class> not assigned in <UWUI_FilterAvatarsPanel> Constructor. Probably the blueprint was renamed?"));
		return;
	}
}

void UWUI_FilterAnimationsPanel::Init(UAvatarsFilterAnimations* Filter)
{
	FilterAnimations = Filter;

	for (FString ClassOption : Filter->AllAvatarClassNames)
	{
		CB_AnimClasses->AddOption(ClassOption);
	}

	for (UOneAnimationData* Animation : Filter->AllAnimationsDatas)
	{
		UWUI_AnimationFilterRow* NewFilterRow = Cast< UWUI_AnimationFilterRow>(CreateWidget(this, AnimationFilterRow_Class));
		NewFilterRow->Init(Animation);

		VB_FilterRows->AddChild(NewFilterRow);

		NewFilterRow->OnCheckedChanged.AddDynamic(Filter, &UAvatarsFilterAnimations::ApplyFilterPrimary);
		NewFilterRow->OnClicked.AddDynamic(Filter, &UAvatarsFilterAnimations::SelectAnimation);

		AllAnimationFilterRows.Add(NewFilterRow);
	}

	CB_AnimClasses->SetSelectedIndex(0);

	Filter->OnAnimationsClassFilterChanged.AddDynamic(this, &UWUI_FilterAnimationsPanel::OnFiltersChanged);
}

void UWUI_FilterAnimationsPanel::OnFiltersChanged()
{
	for (UWUI_AnimationFilterRow* Row : AllAnimationFilterRows)
	{
		ESlateVisibility RowVisibility = Row->AnimationData->bIsFilteredIn ? ESlateVisibility::Visible : ESlateVisibility::Hidden;

		Row->SetVisibility(RowVisibility);
	}
}
