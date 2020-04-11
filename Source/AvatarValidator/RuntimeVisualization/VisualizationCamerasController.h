// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "VisualizationCamerasController.generated.h"


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ECameraType : uint8
{
	Cam_Front 	UMETA(DisplayName = "Front"),
	Cam_Lateral 	UMETA(DisplayName = "Lateral"),
	Cam_45 	UMETA(DisplayName = "45"),
	Cam_Back 	UMETA(DisplayName = "Back"),
	Cam_Free 	UMETA(DisplayName = "Free"),
};

class ACameraActor;
class AVisualizationPawn;

UCLASS(Blueprintable, BlueprintType)
class AvatarVALIDATOR_API AVisualizationCamerasController : public AActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	class UVisualizationConfigDataAsset* Config;

	// Pawn that allows Free Movement Camera
	UPROPERTY(EditDefaultsOnly)
	AVisualizationPawn* VisualizationPawn;

	// CameraActors References (they are picked from the editor, except for the FreeMovement one, which is from a Pawn
	UPROPERTY(EditAnywhere)
	ACameraActor* CameraActor_Front;

	UPROPERTY(EditAnywhere)
	ACameraActor* CameraActor_Lateral;

	UPROPERTY(EditAnywhere)
	ACameraActor* CameraActor_45;

	UPROPERTY(EditAnywhere)
	ACameraActor* CameraActor_Back;

	UPROPERTY()
	AActor* CurrentSelectedCameraActor;

public:

	AVisualizationCamerasController();

	UFUNCTION()
	void Init(AVisualizationPawn* VisPawn);

	UFUNCTION(BlueprintCallable)
	void SelectCameraType(ECameraType CameraType);

};
