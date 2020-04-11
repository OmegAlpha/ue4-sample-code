// Fill out your copyright notice in the Description page of Project Settings.


#include "VisualizationCamerasController.h"
#include "GameFramework/Actor.h"
#include "VisualizationPawn.h"
#include "VisualizationConfigDataAsset.h"
#include "Camera/CameraActor.h"

AVisualizationCamerasController::AVisualizationCamerasController()
{
	static ConstructorHelpers::FObjectFinder<UVisualizationConfigDataAsset>
		ConfigDataAsset(TEXT("VisualizationConfigDataAsset'/Game/Validator/VisualizationConfig.VisualizationConfig'"));

	Config = ConfigDataAsset.Object;
}

void AVisualizationCamerasController::Init(AVisualizationPawn* VisPawn)
{
	VisualizationPawn = VisPawn;
}

void AVisualizationCamerasController::SelectCameraType(ECameraType CameraType)
{
	VisualizationPawn->IsInUse = true;

	AActor* TargetBlendActor = nullptr;

	switch (CameraType)
	{
	case ECameraType::Cam_Front:
		TargetBlendActor = CameraActor_Front;
		break;
	case ECameraType::Cam_Lateral:
		TargetBlendActor = CameraActor_Lateral;
		break;
	case ECameraType::Cam_45:
		TargetBlendActor = CameraActor_45;
		break;
	case ECameraType::Cam_Back:
		TargetBlendActor = CameraActor_Back;
		break;
	case ECameraType::Cam_Free:
		TargetBlendActor = VisualizationPawn;
		break;
	}

 	VisualizationPawn->IsInUse = (TargetBlendActor == VisualizationPawn);

	if (Config->AutoFitFreeCameraToLastCamera && CurrentSelectedCameraActor != nullptr && VisualizationPawn->IsInUse)
	{
		VisualizationPawn->SetActorLocation(CurrentSelectedCameraActor->GetActorLocation());
		VisualizationPawn->SetActorRotation(CurrentSelectedCameraActor->GetActorRotation());
	}

	CurrentSelectedCameraActor = TargetBlendActor;

	APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	PC->SetViewTargetWithBlend(TargetBlendActor, 0.f);
}