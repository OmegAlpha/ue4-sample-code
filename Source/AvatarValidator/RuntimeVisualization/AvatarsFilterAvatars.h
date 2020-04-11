// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RuntimeVisualization/AvatarsFilter.h"
#include "AvatarsFilterAvatars.generated.h"

class AAvatarVisualizationActor;

UCLASS()
class AvatarVALIDATOR_API UAvatarsFilterAvatars : public UAvatarsFilter
{
	GENERATED_BODY()

protected:

	virtual void ConcreteInitialize() override;

	virtual void OnIndexChanged_Avatar() override;

	virtual void OnIndexChanged_Animation() override;

public:

	virtual void StartFilter();

	virtual void ExitFilter();

	virtual void ApplyFilterPrimary() override;

	virtual void ApplyFilterSecondary() override;

	virtual void SelectNextPrimary() override { SelectNext_Avatar(); };

	virtual void SelectFirstPrimary() override { SelectIndex_Avatar(0); }

	virtual void SelectNextSecondary() override { SelectNext_Animation(); };

	virtual void SelectFirstSecondary() override { SelectIndex_Animation(0); };

	virtual int GetPrimarySelectedIndex() const override { return SelectedIndex_Avatars; }

	virtual int GetSecondarySelectedIndex() const override { return SelectedIndex_Animations; }

	virtual int GetPrimaryElementsQuantity() const override { return FilteredAvatarActors.Num(); }

	virtual int GetSecondaryElementsQuantity() const override { return FilteredAnimationDatas.Num(); }
};
