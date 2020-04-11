// Fill out your copyright notice in the Description page of Project Settings.


#include "AvatarVisualizationActor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "VisualizationConfigDataAsset.h"
#include "Animation/AnimationAsset.h"

// Sets default values
AAvatarVisualizationActor::AAvatarVisualizationActor()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UVisualizationConfigDataAsset>
		GlobalConfigDataAsset(TEXT("VisualizationConfigDataAsset'/Game/Validator/VisualizationConfig.VisualizationConfig'"));

	GlobalConfig = GlobalConfigDataAsset.Object;


	RootScene = CreateDefaultSubobject<USceneCaptureComponent>(TEXT("Root"));
	RootComponent = RootScene;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AvatarMesh"));

	Mesh->SetupAttachment(RootScene);
	Mesh->SetRelativeLocation(FVector::ZeroVector);
	Mesh->SetRelativeRotation(FRotator::ZeroRotator);
	Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
}

void AAvatarVisualizationActor::SetConfig(FAvatarConfig NewConfig)
{
	Config = NewConfig;

	Mesh->SetSkeletalMesh(Config.SkeletalMesh);

	MaterialInstance->SetTextureParameterValue("Colour", Config.ColorTexture);
	MaterialInstance->SetTextureParameterValue("CombinationMap", Config.CombinationTexture);
	MaterialInstance->SetTextureParameterValue("Normal", Config.NormalTexture);
	MaterialInstance->SetTextureParameterValue("Specular", Config.SpecularTexture);

	Mesh->SetMaterial(0, MaterialInstance);

	Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	
	OnConfigUpdated.Broadcast(Config);
}

void AAvatarVisualizationActor::BeginPlay()
{
	Super::BeginPlay();

	MaterialInstance = UMaterialInstanceDynamic::Create(MasterMaterial_Avatar, this);
}

void AAvatarVisualizationActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator ActorRotation(0.f, TurnTableSpeedFactor * GlobalConfig->TurnTableMaxSpeed * DeltaTime, 0.f);
	AddActorLocalRotation(ActorRotation);
	
}

void AAvatarVisualizationActor::SetTurnTableSpeedFactor(float Speed)
{
	TurnTableSpeedFactor = Speed;
}

void AAvatarVisualizationActor::TryPlayAnimation(UAnimationAsset* Animation)
{
	Mesh->PlayAnimation(Animation, true);
}
