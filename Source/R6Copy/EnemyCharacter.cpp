// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "R6PlayerCharacter.h"
#include "EnemyCharacter.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EnemyMesh"));
	
	bIsDead = false;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{

	Super::BeginPlay();
	

}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("You have killed %f Enemies"), numbDeath);
	
	
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HitPoints = HitPoints - DamageAmount;
	CurrentHealth = DefaultHealth + HitPoints;
	
	AR6CopyGameModeBase* R6GameMode = Cast<AR6CopyGameModeBase>(GetWorld()->GetAuthGameMode());

	if (HitPoints)
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Has %f HP"), CurrentHealth);
		if (CurrentHealth <= 0)
		{
			
			Destroy();
			bIsDead = true;
			R6GameMode->GetKillCount();
			
			
		}
		
	}

	return 0.f;
}

