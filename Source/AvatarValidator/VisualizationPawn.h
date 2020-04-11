// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "RuntimeVisualization/VisualizationConfigDataAsset.h"
#include "VisualizationPawn.generated.h"

UCLASS()
class AvatarVALIDATOR_API AVisualizationPawn : public APawn
{
	GENERATED_BODY()

public:

	UPROPERTY()
	UCameraComponent* CameraComponent;

	UPROPERTY()
	bool IsInUse;

	UPROPERTY()
	bool IsHoldingToggleMouseLook;

private:

	UPROPERTY()
	UVisualizationConfigDataAsset* Config;

	UPROPERTY()
	FVector InputValues;

	UPROPERTY()
	FVector2D MouseValues;

public:
	AVisualizationPawn();

private:

	void OnAxis_Top(float Value);

	void OnAxis_Right(float Value);

	void OnAxis_Forward(float Value);

	void OnAxis_MouseX(float Value);

	void OnAxis_MouseY(float Value);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
