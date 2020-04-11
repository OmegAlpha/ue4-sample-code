// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AvatarPawnsManager.h"
#include "WUI_AvatarFilterRow.h"
#include "VerticalBox.h"
#include "Button.h"
#include "WUI_AvatarsVisualization.generated.h"

class UWUI_FilterAvatarsPanel;
class UWUI_FilterAnimationsPanel;
class UWUI_CameraControlsPanel;
class UWUI_VideoRecordControlPanel;

UCLASS()
class AvatarVALIDATOR_API UWUI_AvatarsVisualization : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UWUI_PanelPrevNextSelectors* WBP_PanelPrevNextSelectors;	

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWUI_CameraControlsPanel* WBP_CameraControlsPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWUI_VideoRecordControlPanel* WBP_VideoRecordControlPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWUI_FilterAvatarsPanel*  Panel_FilterAvatars;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWUI_FilterAnimationsPanel*  Panel_FilterAnimations;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Button_Filter_Avatars;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Button_Filter_Animations;

	UPROPERTY(BlueprintReadOnly)
	TArray<UWUI_AvatarFilterRow*> AllAvatarsFilterRows;

	UPROPERTY(BlueprintReadOnly)
	AAvatarPawnsManager* AvatarsManager;

public:

	UFUNCTION(BlueprintCallable)
	void Init(AAvatarPawnsManager* Manager);
	
	UFUNCTION()
	void OnVideoRecordFinished();

private:

	UFUNCTION()
	void OnClicked_ShowAnimationsFilter();

	UFUNCTION()
	void OnClicked_ShowAvatarsFilter();
};
