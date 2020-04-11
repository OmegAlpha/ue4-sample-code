// Fill out your copyright notice in the Description page of Project Settings.


#include "AvatarsFilterAvatars.h"
#include "VisualizationConfigDataAsset.h"
#include "AvatarVisualizationActor.h"
#include "AvatarPawnsManager.h"

void UAvatarsFilterAvatars::ConcreteInitialize()
{
	UVisualizationConfigDataAsset* Config = PawnsManager->GetConfig();

	for (AAvatarVisualizationActor* AvatarActor : AllAvatars)
	{
		AvatarActor->IsFilteredIn = Config->FilteredAvatars.Num() == 0 || Config->FilteredAvatars.Contains(AvatarActor->GetConfig().Name);
	}
}

void UAvatarsFilterAvatars::OnIndexChanged_Avatar()
{
	// Fix Selection if Needed
	if (SelectedAvatarActor == nullptr)
	{
		SelectedIndex_Avatars = 0;
	}
	else if (SelectedIndex_Avatars > FilteredAvatarActors.Num() - 1 && FilteredAvatarActors.Num() > 0)
	{
		SelectedIndex_Avatars = FilteredAvatarActors.Num() - 1;
	}

	AAvatarVisualizationActor* TargetAvatarToSelect = nullptr;

	if (FilteredAvatarActors.Num() > 0)
	{
		TargetAvatarToSelect = FilteredAvatarActors[SelectedIndex_Avatars];
	}

	if (SelectedAvatarActor != TargetAvatarToSelect)
	{
		if (SelectedAvatarActor != nullptr)
		{
			SelectedAvatarActor->SetActorHiddenInGame(true);
		}		
	}		

	if (TargetAvatarToSelect != nullptr)
	{
		UOneAnimationData* CurrentAnimationData = SelectedAnimation;

		SelectedAvatarActor = TargetAvatarToSelect;
		SelectedAvatarActor->SetActorHiddenInGame(false);

		// Apply Animation Filter for this Avatar
		ApplyFilterSecondary();

		// After changing a Avatar Selection, if they are of same class, persist the animation that was running
		if (FilteredAnimationDatas.Contains(CurrentAnimationData))
		{
			SelectAnimation(CurrentAnimationData);
		}
	}	
}

void UAvatarsFilterAvatars::OnIndexChanged_Animation()
{
	SelectedAnimation = FilteredAnimationDatas[SelectedIndex_Animations];
	SelectedAvatarActor->TryPlayAnimation(SelectedAnimation->Animation);
}

void UAvatarsFilterAvatars::StartFilter()
{
	bIsSelected = true;

	ApplyFilterPrimary();
	SelectIndex_Avatar(SelectedIndex_Avatars);
}

void UAvatarsFilterAvatars::ExitFilter()
{
	bIsSelected = false;

	if (SelectedAvatarActor != nullptr)
	{
		SelectedAvatarActor->SetActorHiddenInGame(true);
	}
}

void UAvatarsFilterAvatars::ApplyFilterPrimary()
{
	UVisualizationConfigDataAsset* Config = PawnsManager->GetConfig();

	Config->FilteredAvatars.Empty();
	FilteredAvatarActors.Empty();
	FilteredAnimationDatas.Empty();
	
	for (AAvatarVisualizationActor* AvatarActor : AllAvatars)
	{
		if(AvatarActor->IsFilteredIn)
		{
	 		Config->FilteredAvatars.Add(AvatarActor->GetConfig().Name);
	 		FilteredAvatarActors.Add(AvatarActor);
	 	}
	}
	
	if (FilteredAvatarActors.Num() == 0)
	{
		SelectedAvatarActor = nullptr;
	}

	Config->SaveConfig();

	OnIndexChanged_Avatar();
}

void UAvatarsFilterAvatars::ApplyFilterSecondary()
{
	FilteredAnimationDatas.Empty();

	for (UOneAnimationData* AnimationData : AllAnimationsDatas)
	{
		if (AnimationData->AvatarClass == SelectedAvatarActor->GetConfig().Class)
		{
			FilteredAnimationDatas.Add(AnimationData);
		}
	}

	SelectIndex_Animation(0);
}

