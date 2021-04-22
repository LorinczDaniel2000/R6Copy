// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.h"
#include "GameFramework/Pawn.h"
#include "R6PlayerCharacter.generated.h"

class USkeletalMeshComponent;
class UCameraComponent;
class UInputComponent;
class UAnimMontage;
class UCapsuleComponent;
class UParticleSystem;
class AEnemyCharacter;

UCLASS()
class R6COPY_API AR6PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Mesh)
		USkeletalMeshComponent* Ak47_SK;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Mesh)
		USkeletalMeshComponent* Pistol_SK;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Mesh)
		USkeletalMeshComponent* PlayerMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Vault)
		USkeletalMeshComponent* VaultEndPoint;

	UPROPERTY(VisibleAnywhere)
		AActor* EnemyActor;

	/*
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Mesh)
		UCapsuleComponent* CapsuleComponent;*/

public:
	// Sets default values for this character's properties
	AR6PlayerCharacter();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float WeaponRange;

	/* This is multiplied by the direction vector when the weapon trace hits something to apply velocity to the component that is hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float WeaponDamage;

	//this is the speed of the player when we sprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float RunSpeed;

	//this is the basic speed of the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting)
		float BaseDamage;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* FireAnimation;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UParticleSystem* MuzzleParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vault)
		float TraceLenght;

	FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Val);

	void MoveRight(float Val);

	void OnFire();

	void OnReload();

	void Reload();

	void Sprint();

	void Walk();

	void Vault();

	void StartCrouch();

	//Handling primary weapon's ammo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
		int32 PrimaryMaxAmmo;

	UPROPERTY(EditAnywhere, Category = Ammo)
		int32 PrimaryMagMaxAmmo;

		int32 PrimaryCurrentTotalAmmo;
		int32 PrimaryCurrentMagAmmo;

	//Handling secondary weapon's ammo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
		int32 SecondaryMaxAmmo;

	UPROPERTY(EditAnywhere, Category = Ammo)
		int32 SecondaryMagMaxAmmo;

		int32 SecondaryCurrentTotalAmmo;
		int32 SecondaryCurrentMagAmmo;

	
	//Disable Shooting after pressing Reload
	void FireTimerHandle();

	bool bCanShoot;
	float ReloadTime;

	FTimerHandle ReloadTimer;

	//Change weapons
	void WeaponToPrimary();
	void WeaponToSecondary();

	bool bAKVisibility = true;
	bool bPistolVisibility = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



};
