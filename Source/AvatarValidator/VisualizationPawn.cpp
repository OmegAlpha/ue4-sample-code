// Fill out your copyright notice in the Description page of Project Settings.


#include "VisualizationPawn.h"
#include "ConstructorHelpers.h"

// Sets default values
AVisualizationPawn::AVisualizationPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CAMERA");
	this->SetRootComponent(CameraComponent);

	static ConstructorHelpers::FObjectFinder<UVisualizationConfigDataAsset>
		ConfigDataAsset(TEXT("VisualizationConfigDataAsset'/Game/Validator/VisualizationConfig.VisualizationConfig'"));

	Config = ConfigDataAsset.Object;
}

void AVisualizationPawn::BeginPlay()
{
	Super::BeginPlay();	
}

void AVisualizationPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsInUse)
	{
		return;
	}

	FVector MovementVector = GetActorRotation().RotateVector(InputValues);
	MovementVector.Normalize();
	MovementVector *= Config->FreeCamera_MovementSpeed * DeltaTime;

	SetActorLocation(GetActorLocation() + MovementVector);	

	if (IsHoldingToggleMouseLook)
	{
		float Yaw = Config->FreeCamera_RotationSpeed * DeltaTime * MouseValues.X;
		float Pitch = Config->FreeCamera_RotationSpeed * DeltaTime * MouseValues.Y;

		SetActorRotation(GetActorRotation().Add(Pitch, Yaw, 0.f));
	}

	
}

void AVisualizationPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// WASD movement Input setup
	PlayerInputComponent->BindAxis("Axis_Lateral", this, &AVisualizationPawn::OnAxis_Right);
	PlayerInputComponent->BindAxis("Axis_Forward", this, &AVisualizationPawn::OnAxis_Forward);
	PlayerInputComponent->BindAxis("Axis_Top", this, &AVisualizationPawn::OnAxis_Top);

	// Camera rotation with Mouse Input Setup
 	PlayerInputComponent->BindAxis("Axis_MouseX", this, &AVisualizationPawn::OnAxis_MouseX);
 	PlayerInputComponent->BindAxis("Axis_MouseY", this, &AVisualizationPawn::OnAxis_MouseY);

	// Handle ToggleMouseLook Down/Up
	FInputActionBinding ToggleMouseLook_Pressed("ToggleMouseLook", IE_Pressed);
	ToggleMouseLook_Pressed.ActionDelegate.GetDelegateForManualSet().BindLambda([this](){ IsHoldingToggleMouseLook = true;});
	InputComponent->AddActionBinding(ToggleMouseLook_Pressed);

	FInputActionBinding ToggleMouseLook_Released("ToggleMouseLook", IE_Released);
	ToggleMouseLook_Released.ActionDelegate.GetDelegateForManualSet().BindLambda([this]() { IsHoldingToggleMouseLook = false; });
	InputComponent->AddActionBinding(ToggleMouseLook_Released);
}

void AVisualizationPawn::OnAxis_Top(float Value)
{
	InputValues.Z = Value;
}

void AVisualizationPawn::OnAxis_Right(float Value)
{
	InputValues.Y = Value;
}

void AVisualizationPawn::OnAxis_Forward(float Value)
{
	InputValues.X = Value;
}

void AVisualizationPawn::OnAxis_MouseX(float Value)
{
	MouseValues.X = Value;
}

void AVisualizationPawn::OnAxis_MouseY(float Value)
{
	MouseValues.Y = Value;
}

