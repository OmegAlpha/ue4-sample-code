// Fill out your copyright notice in the Description page of Project Settings.


#include "AvatarValidatorGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "RuntimeVisualization/AvatarPawnsManager.h"
#include "Object.h"
#include "RuntimeVisualization/VisualizationCamerasController.h"

AAvatarValidatorGameModeBase::AAvatarValidatorGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Validator/Blueprints/BP_VisualizationPawn"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}


void AAvatarValidatorGameModeBase::BeginPlay()
{	
	TArray<AActor*> FoundManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAvatarPawnsManager::StaticClass(), FoundManagers);

	if (FoundManagers.Num() != 1)
	{
		UE_LOG(LogTemp, Error, TEXT("[AAvatarValidatorGameModeBase] There should be exactly 1 instance of <AAvatarPawnsManager> in the Level"))
		return;
	}

	AAvatarPawnsManager* AvatarsManager = Cast< AAvatarPawnsManager>( FoundManagers[0]);
	AvatarsManager->Initialize();

	AVisualizationPawn* VisPawn = Cast<AVisualizationPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());

	TArray<AActor*> FoundCameraController;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVisualizationCamerasController::StaticClass(), FoundCameraController);

	if (FoundCameraController.Num() != 1)
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("[AAvatarValidatorGameModeBase] Found 0 Actors of type <AVisualizationCamerasController>. Exactly one is required and it needs to have the different cameras assigned to it."))

			return;
	}

	AVisualizationCamerasController* CameraController = Cast< AVisualizationCamerasController>(FoundCameraController[0]);
	CameraController->Init(VisPawn);
}
