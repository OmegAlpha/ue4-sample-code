// Fill out your copyright notice in the Description page of Project Settings.


#include "AvatarPawnsManager.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "AvatarsFilterAvatars.h"
#include "AvatarsFilterAnimations.h"
#include "WUI_AvatarsVisualization.h"

// Sets default values
AAvatarPawnsManager::AAvatarPawnsManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UVisualizationConfigDataAsset>
		ConfigDataAsset(TEXT("VisualizationConfigDataAsset'/Game/Validator/VisualizationConfig.VisualizationConfig'"));

	Config = ConfigDataAsset.Object;
}

// Called when the game starts or when spawned
void AAvatarPawnsManager::Initialize()
{
	AutoSwitchAvatar_TimerDel.BindUFunction(this, FName("SelectNextPrimary"));	

	ConfigsLoader = NewObject<UAvatarConfigsLoader>();
	ConfigsLoader->LoadAll();

	if (AvatarActorClass_BP == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[Validator] No AAvatarVisualizationActor subclass was setup in the Blueprint"));
		return;
	}

	TArray< FAvatarConfig> AvatarConfigs = ConfigsLoader->AllConfigs;
	
	for (FAvatarConfig AvatarConfig : AvatarConfigs)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector SpawnLocation = GetActorLocation();

		//TODO/NOTE: an alternative could be only instantiate the Avatars on demand, to avoid a big spike 
		// if there are tons of Avatars

		AAvatarVisualizationActor* NewAvatarActor = 
			GetWorld()->SpawnActor<AAvatarVisualizationActor>(AvatarActorClass_BP, SpawnLocation, GetActorRotation(), Params);

		NewAvatarActor->SetConfig(AvatarConfig);

		NewAvatarActor->SetActorHiddenInGame(true);

		AllAvatarsActors.Add(NewAvatarActor);
	}

	// Filter Logic Initialization
	FilterAvatars = NewObject<UAvatarsFilterAvatars>();
	FilterAvatars->Initialize(this);

	FilterAnimations = NewObject<UAvatarsFilterAnimations>();
	FilterAnimations->Initialize(this);

	SelectFilter(EFilterType::AvatarS);

	// Video Recorder Initialization
	VideoRecorder = NewObject<UVideoRecorder>();
	VideoRecorder->Initialize(this);

	// Create Main Widget
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	MainWidget = Cast<UWUI_AvatarsVisualization>(CreateWidget(PC, MainWidget_Class));
	MainWidget->AddToViewport();
	MainWidget->Init(this);
}

// Called every frame
void AAvatarPawnsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAvatarPawnsManager::ToggleAutoAvatarSwitch(bool IsOn)
{
	SettingsState.bAutoSwitchAvatars = IsOn;

	if (IsOn)
	{
		GetWorld()->GetTimerManager().SetTimer(
			AutoSwitchAvatar_TimerHandle, 
			AutoSwitchAvatar_TimerDel,
			Config->AutoSwitchAvatarDelay,
			true);

	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(AutoSwitchAvatar_TimerHandle);
	}
}



void AAvatarPawnsManager::SetTurnTableRatio(float Value)
{
	TurnTableRatio = Value;
	SettingsState.TurnTableSpeed = Value;
	SelectedFilter->GetSelectedAvatarActor()->SetTurnTableSpeedFactor(Value);
}

void AAvatarPawnsManager::SelectFilter(EFilterType FilterType)
{
	UAvatarsFilter* PreviousFilter = SelectedFilter;	

	switch (FilterType)
	{
		case EFilterType::ANIMATIONS: 
			SelectedFilter = FilterAnimations;
			break;
		case EFilterType::AvatarS:
			SelectedFilter = FilterAvatars;
			break;
	}

	if (PreviousFilter != SelectedFilter)
	{
		if (PreviousFilter != nullptr)
		{
			PreviousFilter->ExitFilter();
		}

		SelectedFilter->StartFilter();
	}	
}

void AAvatarPawnsManager::SelectFirstPrimary()
{
	SelectedFilter->SelectFirstPrimary();
}

void AAvatarPawnsManager::SelectNextPrimary()
{
	SelectedFilter->SelectNextPrimary();
}

void AAvatarPawnsManager::SelectNext_Avatar()
{
	SelectedFilter->SelectNext_Avatar();
}

void AAvatarPawnsManager::SelectPrevious_Avatar()
{
	SelectedFilter->SelectPrevious_Avatar();
}

void AAvatarPawnsManager::SelectNext_Animation()
{
	SelectedFilter->SelectNext_Animation();
}

void AAvatarPawnsManager::SelectPrevious_Animation()
{
	SelectedFilter->SelectPrevious_Animation();
}
