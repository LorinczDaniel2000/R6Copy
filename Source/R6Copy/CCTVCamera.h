// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCTVCamera.generated.h"


UCLASS()
class R6COPY_API ACCTVCamera : public AActor
{
	GENERATED_BODY()

public:

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CCTV, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* CCTVCameraComponent;*/

	
public:	
	// Sets default values for this actor's properties
	ACCTVCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
