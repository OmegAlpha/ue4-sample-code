// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WUI_FilterAvatarsPanel.generated.h"

class UVerticalBox;
class UWUI_AvatarFilterRow;
class UAvatarsFilterAvatars;
class AAvatarVisualizationActor;

UCLASS()
class AvatarVALIDATOR_API UWUI_FilterAvatarsPanel : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	TArray< UWUI_AvatarFilterRow*> AllAvatarFilterRows;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UWUI_AvatarFilterRow> AvatarFilterRow_Class;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* VB_FilterRows;

	UPROPERTY()
	UAvatarsFilterAvatars* FilterAvatars;

public:

	UWUI_FilterAvatarsPanel(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void Init(UAvatarsFilterAvatars* FilterLogic);
};
