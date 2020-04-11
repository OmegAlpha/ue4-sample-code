// Fill out your copyright notice in the Description page of Project Settings.


#include "AvatarsFilterAnimations.h"
#include "AssetRegistryModule.h"
#include "AssetData.h"
#include "AvatarVisualizationActor.h"
#include "AvatarConfig.h"
#include "AvatarPawnsManager.h"

void UAvatarsFilterAnimations::ConcreteInitialize()
{
	for (UOneAnimationData* AnimationData : AllAnimationsDatas)
	{
		AnimationData->bIsFilteredIn = true;
	}
}

void UAvatarsFilterAnimations::OnIndexChanged_Avatar()
{
	if (SelectedAnimation == nullptr)
	{
		return;
	}		

	if (SelectedAvatarActor != nullptr)
	{
		SelectedAvatarActor->SetActorHiddenInGame(true);
	}

	SelectedAvatarActor = FilteredAvatarActors[SelectedIndex_Avatars];
	SelectedAvatarActor->SetActorHiddenInGame(false);

	SelectedAvatarActor->TryPlayAnimation(SelectedAnimation->Animation);
}

void UAvatarsFilterAnimations::OnIndexChanged_Animation()
{
	SelectedAnimation = FilteredAnimationDatas.Num() > 0 || SelectedIndex_Animations < FilteredAnimationDatas.Num() ?
		FilteredAnimationDatas[SelectedIndex_Animations] : nullptr;

	ApplyFilterSecondary();
}

void UAvatarsFilterAnimations::StartFilter()
{
	bIsSelected = true;

	ApplyFilterPrimary();
	SelectIndex_Animation(SelectedIndex_Animations);
}

void UAvatarsFilterAnimations::ExitFilter()
{
	bIsSelected = false;

	if (SelectedAvatarActor != nullptr)
	{
		SelectedAvatarActor->SetActorHiddenInGame(true);
	}
}

void UAvatarsFilterAnimations::ApplyFilterPrimary()
{
	FilteredAnimationDatas.Empty();

	for (UOneAnimationData* AnimationData : AllAnimationsDatas)
	{
		if (AnimationData->bIsFilteredIn)
		{
			FilteredAnimationDatas.Add(AnimationData);
		}
	}

	OnIndexChanged_Animation();
}

void UAvatarsFilterAnimations::ApplyFilterSecondary()
{
	FilteredAvatarActors.Empty();

	if (SelectedAnimation != nullptr)
	{
		for (AAvatarVisualizationActor* AvatarActor : AllAvatars)
		{
			if (AvatarActor->GetConfig().Class == SelectedAnimation->AvatarClass)
			{
				FilteredAvatarActors.Add(AvatarActor);
			}
		}
	}	

	if (FilteredAvatarActors.Num() == 0 && SelectedAvatarActor != nullptr)
	{
		SelectedAvatarActor->SetActorHiddenInGame(true);
	}

	SelectIndex_Avatar(0);
}

void UAvatarsFilterAnimations::OnClassIndexChanged(FString SelectedClassName)
{
	for (UOneAnimationData* AnimationData : AllAnimationsDatas)
	{
		AnimationData->bIsFilteredIn = SelectedClassName == AllAvatarClassNames[0] || AnimationData->AvatarClass == SelectedClassName;
	}

	ApplyFilterPrimary();

	OnAnimationsClassFilterChanged.Broadcast();
}