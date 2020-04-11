// Fill out your copyright notice in the Description page of Project Settings.


#include "AvatarsFilter.h"
#include "AvatarPawnsManager.h"
#include "AssetRegistryModule.h"
#include "ARFilter.h"
#include "Animation/AnimationAsset.h"
#include "ImportUtils.h"

FString UAvatarsFilter::GetClassPrefix(FString Orig) const
{
	TArray<FString> Out;
	Orig.ParseIntoArray(Out, TEXT("_"), true);
	return Out[0];
}

void UAvatarsFilter::InitializeAvatars()
{
	AllAvatars.Append(PawnsManager->AllAvatarsActors);

	for (AAvatarVisualizationActor* AvatarActor : AllAvatars)
	{
		FString AvatarClassName = AvatarActor->GetConfig().Class;
		if (!AllAvatarClassNames.Contains(AvatarClassName))
		{
			AllAvatarClassNames.Add(AvatarClassName);
		}
	}
}

void UAvatarsFilter::InitializeAnimations()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetData;

	FARFilter Filter;
	Filter.ClassNames.Add(FName(*UAnimationAsset::StaticClass()->GetName()));
	Filter.PackagePaths.Add(*FImportUtils::GetAvatarClassesPath());
	Filter.bRecursivePaths = true;
	Filter.bRecursiveClasses = true;

	AssetRegistryModule.Get().GetAssets(Filter, AssetData);	

	for (FAssetData Data : AssetData)
	{
		FString AssetName = *Data.AssetName.ToString();

		FString AvatarClass = GetClassPrefix(AssetName);

		if (!AllAvatarClassNames.Contains(AvatarClass))
		{
			AllAvatarClassNames.Add(AvatarClass);
		}

		UAnimationAsset* AnimSequence = Cast<UAnimationAsset>(Data.GetAsset());

		UOneAnimationData* NewAnimationData = NewObject< UOneAnimationData>();
		NewAnimationData->Animation = AnimSequence;
		NewAnimationData->AvatarClass = AvatarClass;
		NewAnimationData->bIsFilteredIn = true;
		AllAnimationsDatas.Add(NewAnimationData);
	}
}

FString UAvatarsFilter::GetSelectedAnimationName() const
{
	if (SelectedAnimation == nullptr)
		return TEXT("none");

	FString AssetName;
	SelectedAnimation->Animation->GetName(AssetName);
	return AssetName;
}

FString UAvatarsFilter::GetSelectedAvatarName() const
{
	if (SelectedAvatarActor == nullptr)
		return TEXT("none");

	return SelectedAvatarActor->GetConfig().Name;
}

void UAvatarsFilter::Initialize(AAvatarPawnsManager * PawnsMan)
{
	PawnsManager = PawnsMan;

	AllAvatarClassNames.Add(FString("All"));

	// Load All Avatars
	InitializeAvatars();

	// Load All Animations
	InitializeAnimations();	

	ConcreteInitialize();
}

void UAvatarsFilter::SelectIndex_Avatar(int Index)
{
	if (!bIsSelected)
	{
		return;
	}

	if (FilteredAvatarActors.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Avatars Found for this Animation"));
		SelectedAvatarActor = nullptr;
		return;
	}

	if (Index < 0 || Index > FilteredAvatarActors.Num() - 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to select a Avatar Index that is out of bounds"));
		SelectedAvatarActor = nullptr;
		return;
	}

	SelectedIndex_Avatars = Index;

	OnIndexChanged_Avatar();
}

void UAvatarsFilter::SelectNext_Avatar()
{
	if (!bIsSelected)
	{
		return;
	}

	SelectedIndex_Avatars++;

	if (SelectedIndex_Avatars > FilteredAvatarActors.Num() - 1)
	{
		SelectedIndex_Avatars = 0;
	}		

	SelectIndex_Avatar(SelectedIndex_Avatars);
}

void UAvatarsFilter::SelectPrevious_Avatar()
{
	if (!bIsSelected)
	{
		return;
	}

	SelectedIndex_Avatars--;

	if (SelectedIndex_Avatars < 0)
	{
		SelectedIndex_Avatars = FilteredAvatarActors.Num() - 1;
	}		

	SelectIndex_Avatar(SelectedIndex_Avatars);
}

void UAvatarsFilter::SelectIndex_Animation(int Index)
{
	if (!bIsSelected)
	{
		return;
	}

	if (FilteredAnimationDatas.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Animations Found for this Avatar"));
		SelectedAnimation = nullptr;
		return;
	}

	if (Index < 0 || Index > FilteredAnimationDatas.Num() - 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to select an Animation Index that is out of bounds"));
		SelectedAnimation = nullptr;
		return;
	}

	SelectedIndex_Animations = Index;

	OnIndexChanged_Animation();
}

void UAvatarsFilter::SelectNext_Animation()
{
	if (!bIsSelected)
	{
		return;
	}

	SelectedIndex_Animations++;

	if (SelectedIndex_Animations > FilteredAnimationDatas.Num() - 1)
	{
		SelectedIndex_Animations = 0;
	}
		

	SelectIndex_Animation(SelectedIndex_Animations);
}

void UAvatarsFilter::SelectPrevious_Animation()
{
	if (!bIsSelected)
	{
		return;
	}

	SelectedIndex_Animations--;
	if (SelectedIndex_Animations < 0)
		SelectedIndex_Animations = FilteredAnimationDatas.Num() - 1;

	SelectIndex_Animation(SelectedIndex_Animations);
}

void UAvatarsFilter::SelectAvatar(AAvatarVisualizationActor* AvatarActor)
{
	for (int i = 0; i < FilteredAvatarActors.Num(); i++)
	{
		if (FilteredAvatarActors[i] == AvatarActor)
		{
			SelectIndex_Avatar(i);
			break;
		}
	}
}

void UAvatarsFilter::SelectAnimation(UOneAnimationData* AnimationData)
{
	for (int i = 0; i < FilteredAnimationDatas.Num(); i++)
	{
		if (FilteredAnimationDatas[i] == AnimationData)
		{
			SelectIndex_Animation(i);
			break;
		}
	}
}
