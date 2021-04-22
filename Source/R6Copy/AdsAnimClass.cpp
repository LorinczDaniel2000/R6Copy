// Fill out your copyright notice in the Description page of Project Settings.


#include "AdsAnimClass.h"

UAdsAnimClass::UAdsAnimClass()
{
	isAimed = false;
}

void UAdsAnimClass::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AActor* OwningActor = GetOwningActor();

	/*if (OwningActor != NULL)
	{
		Speed = OwningActor->GetVelocity().Size();
	
	}*/
	
	if (isAimed == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("isAimed is true"));

	}
}
