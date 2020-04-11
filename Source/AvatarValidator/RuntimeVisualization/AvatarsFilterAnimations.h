// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AvatarsFilter.h"
#include "SlateEnums.h"
#include "AvatarsFilterAnimations.generated.h"

class UAnimSequence;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnimationsClassFilterChanged);

UCLASS()
class AvatarVALIDATOR_API UAvatarsFilterAnimations : public UAvatarsFilter
{
	GENERATED_BODY()

public:

	UPROPERTY()
	int SelectedClassFilterIndex;

	UPROPERTY()
	FOnAnimationsClassFilterChanged OnAnimationsClassFilterChanged;

protected:

	virtual void ConcreteInitialize() override;

	virtual void OnIndexChanged_Avatar() override;

	virtual void OnIndexChanged_Animation() override;	

	virtual void ApplyFilterSecondary() override;

public:

	virtual void StartFilter();

	virtual void ExitFilter();

	virtual void SelectNextPrimary() override { SelectNext_Animation(); };

	virtual void SelectFirstPrimary() override { SelectIndex_Animation(0); }

	virtual void SelectNextSecondary() override { SelectNext_Avatar(); };

	virtual void SelectFirstSecondary() override { SelectIndex_Avatar(0); };

	virtual void ApplyFilterPrimary() override;

	virtual int GetPrimarySelectedIndex() const override { return SelectedIndex_Animations; }
	
	virtual int GetSecondarySelectedIndex() const override { return SelectedIndex_Avatars; }

	virtual int GetPrimaryElementsQuantity() const override { return FilteredAnimationDatas.Num(); }

	virtual int GetSecondaryElementsQuantity() const override { return FilteredAvatarActors.Num(); }

	UFUNCTION(BlueprintCallable)
	void OnClassIndexChanged(FString SelectedClassName);
};
