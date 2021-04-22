// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PlayerController.h"

#include "WindowObject.h"

// Sets default values
AWindowObject::AWindowObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



}

// Called when the game starts or when spawned
void AWindowObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWindowObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

