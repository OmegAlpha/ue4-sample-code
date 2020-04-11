// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WUI_VideoRecordControlPanel.generated.h"

class UButton;
class AAvatarPawnsManager;

UCLASS()
class AvatarVALIDATOR_API UWUI_VideoRecordControlPanel : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	AAvatarPawnsManager* AvatarsManager;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Btn_RecordThis;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Btn_RecordAll;

public:

	UFUNCTION()
	void Init(AAvatarPawnsManager* AvatarsManagerPtr);

	UFUNCTION()
	void OnVideoRecordStarted();

	UFUNCTION()
	void OnVideoRecordFinished();
};
