// Fill out your copyright notice in the Description page of Project Settings.

#include "CCTVCamera.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"


// Sets default values
ACCTVCamera::ACCTVCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*CCTVCameraComponent->CreateDefaultSubobject<UCameraComponent>(TEXT("CCTVCameras"));
	CCTVCameraComponent->SetupAttachment(GetRootComponent());
	CCTVCameraComponent->SetRelativeLocation(FVector(0, 0, 0.f)); // Position the camera
	CCTVCameraComponent->bUsePawnControlRotation = true;*/
}

// Called when the game starts or when spawned
void ACCTVCamera::BeginPlay()
{
	Super::BeginPlay();

	
	
}

// Called every frame
void ACCTVCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

