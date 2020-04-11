// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WUI_PanelPrevNextSelectors.generated.h"

class UButton;
class AAvatarPawnsManager;
class UTextBlock;

UCLASS()
class AvatarVALIDATOR_API UWUI_PanelPrevNextSelectors : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Button_Avatar_Prev;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Button_Avatar_Next;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Button_Anim_Prev;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Button_Anim_Next;

	UPROPERTY(BlueprintReadOnly)
	AAvatarPawnsManager* AvatarsManager;

public:

	void Init(AAvatarPawnsManager* Manager);	

	UFUNCTION()
	void OnClicked_AvatarPrev();

	UFUNCTION()
	void OnClicked_AvatarNext();

	UFUNCTION()
	void OnClicked_AnimPrev();

	UFUNCTION()
	void OnClicked_AnimNext();
};
