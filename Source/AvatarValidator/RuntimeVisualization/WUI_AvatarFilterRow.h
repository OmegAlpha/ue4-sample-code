// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CheckBox.h"
#include "TextBlock.h"
#include "Image.h"
#include "Button.h"
#include "RuntimeDelegates.h"
#include "WUI_AvatarFilterRow.generated.h"

class AAvatarVisualizationActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClicked_AvatarFilterRow, AAvatarVisualizationActor*, Animation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAvatarRowCheckChanged);

UCLASS()
class AvatarVALIDATOR_API UWUI_AvatarFilterRow : public UUserWidget
{
	GENERATED_BODY()

public:

	FOnAvatarRowCheckChanged OnCheckedChanged;

	FOnClicked_AvatarFilterRow OnClickedViewThis;

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Img_BackgroundSelected;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCheckBox* CheckBox_Selected;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Text_AvatarName;	

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Btn_ViewThis;	

	UPROPERTY(BlueprintReadOnly)
	AAvatarVisualizationActor* AvatarActor;

public:

	UFUNCTION()
	void SetSelected(bool Selected);

	UFUNCTION(BlueprintCallable)
	void Init(AAvatarVisualizationActor* Avatar);

	UFUNCTION()
	void OnCheckBoxChanged(bool Value);
	
	UFUNCTION()
	void ViewThis();
};
