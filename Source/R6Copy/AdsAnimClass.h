// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "AdsAnimClass.generated.h"

UCLASS(transient, Blueprintable, hideCategories=AnimInstance, BlueprintType)

class UAdsAnimClass : public UAnimInstance
{
	GENERATED_BODY()

public: 
	UAdsAnimClass();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		bool isAimed;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


};

