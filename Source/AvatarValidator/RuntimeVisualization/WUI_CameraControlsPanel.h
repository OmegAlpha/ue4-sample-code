// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WUI_CameraControlsPanel.generated.h"

class AAvatarPawnsManager;

UCLASS()
class AvatarVALIDATOR_API UWUI_CameraControlsPanel : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly)
	class AVisualizationCamerasController* CamController;

	UPROPERTY(BlueprintReadOnly)
	AAvatarPawnsManager* AvatarsManager;

public:

	void Init(AAvatarPawnsManager* Manager);
	
};
