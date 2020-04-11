// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AvatarsFilter.generated.h"

class AAvatarPawnsManager;
class UAnimSequence;

UCLASS()
class AvatarVALIDATOR_API UOneAnimationData : public UObject
{
	GENERATED_BODY()

public:

	// if true, this AnimationData should be included in the filter
	UPROPERTY()
	bool bIsFilteredIn;

	UPROPERTY()
	UAnimationAsset* Animation = nullptr;

	UPROPERTY()
	FString AvatarClass;
};

UCLASS()
class AvatarVALIDATOR_API UAvatarsFilter : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TArray< AAvatarVisualizationActor*> AllAvatars;

	UPROPERTY()
	TArray<UOneAnimationData*> AllAnimationsDatas;

	UPROPERTY()
	TArray<FString> AllAvatarClassNames;

protected:		

	UPROPERTY()
	bool bIsSelected = false;

	UPROPERTY()
	AAvatarVisualizationActor* SelectedAvatarActor;

	UPROPERTY()
	UOneAnimationData* SelectedAnimation;

	UPROPERTY()
	int SelectedIndex_Avatars;

	UPROPERTY()
	int SelectedIndex_Animations;

	UPROPERTY()
	AAvatarPawnsManager* PawnsManager;

	UPROPERTY(BlueprintReadOnly)
	TArray<AAvatarVisualizationActor*> FilteredAvatarActors;

	UPROPERTY(BlueprintReadOnly)
	TArray<UOneAnimationData*> FilteredAnimationDatas;

public:

	UFUNCTION()
    FString GetSelectedAnimationName() const;

	UFUNCTION()
	FString GetSelectedAvatarName() const;

	UFUNCTION()
	virtual void StartFilter() {};

	UFUNCTION()
	virtual void ExitFilter() {};

	UFUNCTION()
	FORCEINLINE AAvatarVisualizationActor* GetSelectedAvatarActor() const { return SelectedAvatarActor; };

	UFUNCTION()
	FORCEINLINE UOneAnimationData* GetSelectedAnimationData() const { return SelectedAnimation; };

	UFUNCTION()
	FORCEINLINE int GetSelectedIndex() const { return SelectedIndex_Avatars;}

	UFUNCTION()
	virtual void Initialize(AAvatarPawnsManager* PawnsMan) final;

	UFUNCTION()
	virtual void ApplyFilterPrimary() {}

	UFUNCTION()
	virtual void ApplyFilterSecondary() {}

	UFUNCTION(BlueprintCallable)
	virtual void SelectNextPrimary() {}

	UFUNCTION()
	virtual void SelectFirstPrimary() {};

	UFUNCTION(BlueprintCallable)
	virtual void SelectNextSecondary() {}

	UFUNCTION()
	virtual void SelectFirstSecondary() {};

	UFUNCTION()
	virtual int GetPrimarySelectedIndex() const { return 0; }

	UFUNCTION()
	virtual int GetSecondarySelectedIndex() const { return 0; }

	UFUNCTION()
	virtual int GetPrimaryElementsQuantity() const { return 0; }

	UFUNCTION()
	virtual int GetSecondaryElementsQuantity() const { return 0; }

	// Avatar Selection Methods
	UFUNCTION(BlueprintCallable)
	void SelectIndex_Avatar(int Index);	

	UFUNCTION(BlueprintCallable)
	void SelectNext_Avatar();

	UFUNCTION(BlueprintCallable)
	void SelectPrevious_Avatar();

	// Animation Selection Methods
	UFUNCTION(BlueprintCallable)
	void SelectIndex_Animation(int Index);

	UFUNCTION(BlueprintCallable)
	void SelectNext_Animation();

	UFUNCTION(BlueprintCallable)
	void SelectPrevious_Animation();

	UFUNCTION()
	void SelectAvatar(AAvatarVisualizationActor* AvatarActor);

	UFUNCTION()
	void SelectAnimation(UOneAnimationData* AnimationData);

protected:

	UFUNCTION()
	FString GetClassPrefix(FString Orig) const;

	UFUNCTION()
	virtual void ConcreteInitialize() {}

	UFUNCTION()
	virtual void OnIndexChanged_Avatar() {}

	UFUNCTION()
	virtual void OnIndexChanged_Animation() {}

private:

	void InitializeAvatars();

	void InitializeAnimations();
};
