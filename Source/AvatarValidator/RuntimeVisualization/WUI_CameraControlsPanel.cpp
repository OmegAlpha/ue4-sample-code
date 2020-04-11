// Fill out your copyright notice in the Description page of Project Settings.


#include "WUI_CameraControlsPanel.h"
#include "VisualizationCamerasController.h"
#include "UserWidget.h"
#include "Kismet/GameplayStatics.h"

void UWUI_CameraControlsPanel::Init(AAvatarPawnsManager* Manager)
{
	AvatarsManager = Manager;

	TArray<AActor*> FoundCameraController;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVisualizationCamerasController::StaticClass(), FoundCameraController);

	if (FoundCameraController.Num() != 1)
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("[UWUI_CameraControlsPanel] Found 0 Actors of type <AVisualizationCamerasController>. Exactly one is required and it needs to have the different cameras assigned to it."))

		return;
	}

	CamController = Cast< AVisualizationCamerasController>(FoundCameraController[0]);
}
