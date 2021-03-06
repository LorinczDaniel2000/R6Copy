// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "R6PlayerCharacter.h"
#include "R6CopyGameModeBase.h"
#include "EnemyCharacter.generated.h"


class USkeletalMeshComponent;
class AR6PlayerCharacter;
class AR6CopyGameModeBase;

UCLASS()
class R6COPY_API AEnemyCharacter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyCharacter();

	UPROPERTY(VisibleDefaultsOnly, Category = EnemyMesh)
		USkeletalMeshComponent* EnemyMesh;

public:

	bool bIsDead;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float HitPoints;

	float DefaultHealth = 100;
	float CurrentHealth;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	
	
};
