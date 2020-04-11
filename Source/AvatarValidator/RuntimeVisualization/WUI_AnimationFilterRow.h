// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Image.h"
#include "CheckBox.h"
#include "TextBlock.h"
#include "Button.h"
#include "AvatarPawnsManager.h"
#include "Animation/AnimSequence.h"
#include "RuntimeDelegates.h"
#include "AvatarsFilterAnimations.h"
#include "WUI_AnimationFilterRow.generated.h"

class UAnimSequence;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClicked_AnimationFilterRow, UOneAnimationData*, AnimationData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnimationRowCheckChanged);

UCLASS()
class AvatarVALIDATOR_API UWUI_AnimationFilterRow : public UUserWidget
{
	GENERATED_BODY()

public:

	FOnAnimationRowCheckChanged OnCheckedChanged;

	FOnClicked_AnimationFilterRow OnClicked;

	UPROPERTY(BlueprintReadOnly)
	UOneAnimationData* AnimationData;

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Img_BackgroundSelected;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCheckBox* CheckBox_Selected;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Text_AnimationName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Btn_ViewThis;

public:

	UFUNCTION()
	void SetSelected(bool Selected);

	UFUNCTION(BlueprintCallable)
	void Init(UOneAnimationData* AnimData);

	UFUNCTION()
	void OnCheckBoxChanged(bool Value);

	UFUNCTION()
	void ViewThis();
};
