// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextBlock.h"
#include "AvatarsFilterAnimations.h"
#include "WUI_FilterAnimationsPanel.generated.h"

class UVerticalBox;
class UWUI_AnimationFilterRow;
class UAvatarsFilterAnimations;
class UComboBoxString;

UCLASS()
class AvatarVALIDATOR_API UWUI_FilterAnimationsPanel : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	TArray<UWUI_AnimationFilterRow*> AllAnimationFilterRows;

	UPROPERTY(BlueprintReadOnly)
	UAvatarsFilterAnimations* FilterAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UWUI_AnimationFilterRow> AnimationFilterRow_Class;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* VB_FilterRows;	

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* CB_AnimClasses;

public:

	UWUI_FilterAnimationsPanel(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void Init(UAvatarsFilterAnimations* Filter);

private:

	UFUNCTION()
	void OnFiltersChanged();
};
