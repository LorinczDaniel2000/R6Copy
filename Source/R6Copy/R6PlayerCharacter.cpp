// Fill out your copyright notice in the Descrption page of Project Settings.

#include "Math/Vector.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "R6PlayerCharacter.h"
#include "Particles/ParticleSystem.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"

#define COLLISION_WEAPON		ECC_GameTraceChannel1


// Sets default values
AR6PlayerCharacter::AR6PlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0, 0, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	PlayerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerMesh"));
	PlayerMesh->SetOnlyOwnerSee(true);				// Set so only owner can see mesh
	PlayerMesh->SetupAttachment(FirstPersonCameraComponent);	// Attach mesh to FirstPersonCameraComponent
	PlayerMesh->bCastDynamicShadow = false;			// Disallow mesh to cast dynamic shadows
	PlayerMesh->CastShadow = false;				// Disallow mesh to cast other shadows

	
	// Create the AK gun mesh component
	Ak47_SK = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Ak47"));
	Ak47_SK->SetOnlyOwnerSee(true);			// Only the owning player will see this mesh
	Ak47_SK->bCastDynamicShadow = false;		// Disallow mesh to cast dynamic shadows
	Ak47_SK->CastShadow = false;			// Disallow mesh to cast other shadows
	Ak47_SK->SetupAttachment(PlayerMesh, TEXT("GripPoint"));


	// Create the pistol gun mesh component
	Pistol_SK = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pistol"));
	Pistol_SK->SetOnlyOwnerSee(true);
	Pistol_SK->bCastDynamicShadow = false;
	Pistol_SK->CastShadow = true;
	Pistol_SK->SetupAttachment(PlayerMesh, TEXT("GripPoint"));



	// Create an End Point for the vault trace
	VaultEndPoint = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VaultEndPoint"));
	VaultEndPoint->SetOnlyOwnerSee(false);			// Only the owning player will see this mesh
	VaultEndPoint->bCastDynamicShadow = false;		// Disallow mesh to cast dynamic shadows
	VaultEndPoint->CastShadow = false;			// Disallow mesh to cast other shadows
	VaultEndPoint->SetupAttachment(Ak47_SK, TEXT("VaultEndPoint"));

	WalkSpeed = 400.f;
	RunSpeed = 900.f;
	
	PrimaryMaxAmmo = 255;
	PrimaryMagMaxAmmo = 30;

	SecondaryMaxAmmo = 60;
	SecondaryMagMaxAmmo = 15;

	bCanShoot = true;
	ReloadTime = 3.f;

	
}

// Called when the game starts or when spawned
void AR6PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PrimaryCurrentTotalAmmo = PrimaryMaxAmmo;
	PrimaryCurrentMagAmmo = PrimaryMagMaxAmmo;

	SecondaryCurrentTotalAmmo = SecondaryMaxAmmo;
	SecondaryCurrentMagAmmo = SecondaryMagMaxAmmo;

	

	Pistol_SK->SetVisibility(bPistolVisibility);
	Ak47_SK->SetVisibility(bAKVisibility);

}





// Called every frame
void AR6PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	void Vault();

}

// Called to bind functionality to input
void AR6PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Set up gameplay key bindings

	//Bind sprint event
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AR6PlayerCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AR6PlayerCharacter::Walk);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AR6PlayerCharacter::OnFire);



	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AR6PlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AR6PlayerCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	//Bind Jump action when we reach a jumpable object
	PlayerInputComponent->BindAction("Vault", IE_Pressed, this, &AR6PlayerCharacter::Vault);

	//Crouch
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AR6PlayerCharacter::StartCrouch);

	//Reload
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AR6PlayerCharacter::OnReload);

	//ChangeWeapon(if A weapon equpped switch to be)
	PlayerInputComponent->BindAction("ToPrimary", IE_Pressed, this, &AR6PlayerCharacter::WeaponToPrimary);
	PlayerInputComponent->BindAction("ToSecondary", IE_Pressed, this, &AR6PlayerCharacter::WeaponToSecondary);

}

void AR6PlayerCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// Add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AR6PlayerCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// Add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}


FHitResult AR6PlayerCharacter::WeaponTrace(const FVector& StartTrace, const FVector& EndTrace) const
{
	// Perform trace to retrieve hit info
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), true, GetInstigator());
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, COLLISION_WEAPON, TraceParams);

	return Hit;
}

void  AR6PlayerCharacter::OnFire()
{
	
	if (bCanShoot == true)
	{
		if (PrimaryCurrentMagAmmo >= 0)
		{
			if (FireAnimation != NULL)
			{
				// Get the animation object for the arms mesh
				UAnimInstance* AnimInstance = PlayerMesh->GetAnimInstance();
				if (AnimInstance != NULL)
				{

					AnimInstance->Montage_Play(FireAnimation, 1.f);

				}
		
			}
			
			// Now send a trace from the end of our gun to see if we should hit anything
			APlayerController* PlayerController = Cast<APlayerController>(GetController());


			FVector ShootDir = FVector::ZeroVector;
			FVector StartTrace = FVector::ZeroVector;
		

			if (PlayerController)
			{
				// Calculate the direction of fire and the start location for trace
				FRotator CamRot;
				PlayerController->GetPlayerViewPoint(StartTrace, CamRot);
				ShootDir = CamRot.Vector();

				// Adjust trace so there is nothing blocking the ray between the camera and the pawn, and calculate distance from adjusted start
				StartTrace = StartTrace + ShootDir * ((GetActorLocation() - StartTrace) | ShootDir);

			}
			
			// Calculate endpoint of trace
			const FVector EndTrace = StartTrace + ShootDir * WeaponRange;

			// Check for impact
			const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);

			// Deal with impact
			AActor* DamagedActor = Impact.GetActor();
			UPrimitiveComponent* DamagedComponent = Impact.GetComponent();

			if (MuzzleParticles)
			{
				if (DamagedActor != NULL)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
						ImpactParticles,
						FTransform(Impact.ImpactNormal.Rotation(),
							Impact.ImpactPoint));
				}

			}
			

			// If we hit an actor, with a component that is simulating physics, apply an impulse
			if ((DamagedComponent->IsSimulatingPhysics()) && (DamagedComponent != NULL))
			{
				DamagedComponent->AddImpulseAtLocation(ShootDir * WeaponDamage, Impact.Location);
			}

			
			if (DamagedActor == nullptr) { return; }

			//add inpulose on little props
			if (DamagedActor != nullptr || DamagedComponent != NULL)
			{
				if (DamagedActor->ActorHasTag(TEXT("TagProp"))) { DamagedComponent->AddImpulseAtLocation(ShootDir * WeaponDamage, Impact.Location); }

			}


			//Handle ammo differently (Primary and Secondary)
			if (bAKVisibility == true)
			{
				PrimaryCurrentMagAmmo = PrimaryCurrentMagAmmo - 1;
				UE_LOG(LogTemp, Warning, TEXT("Primary Ammo %d"), PrimaryCurrentMagAmmo);

			}
			else if (!bAKVisibility)
			{
				SecondaryCurrentMagAmmo = SecondaryCurrentMagAmmo - 1;
				UE_LOG(LogTemp, Warning, TEXT("Secondary Ammo %d"), SecondaryCurrentMagAmmo);
			}

			//Check if there is any ammo loaded in the gun
			if (PrimaryCurrentMagAmmo <= 0) { /*UE_LOG(LogTemp, Warning, TEXT("We are out of ammo"));*/ return; }

			
			if (DamagedActor != this && DamagedActor != nullptr)
			{
				//UE_LOG(LogTemp, Warning, TEXT("We Hit: %s ,now deal dmg to it"), *GetNameSafe(DamagedActor));
				UGameplayStatics::ApplyPointDamage(DamagedActor,
					30.f,
					GetVelocity().GetSafeNormal(),
					Impact,
					GetInstigatorController(),
					this,
					UDamageType::StaticClass());

			}
			else
			{
				return;
			}
		}
		else
		{
			OnReload();
		}
		if (SecondaryCurrentMagAmmo >= 0)
			{
				if (FireAnimation != NULL)
				{
					// Get the animation object for the arms mesh
					UAnimInstance* AnimInstance = PlayerMesh->GetAnimInstance();
					if (AnimInstance != NULL)
					{

						AnimInstance->Montage_Play(FireAnimation, 1.f);

					}

				}
			}
		if (SecondaryCurrentMagAmmo <= 0) { return; }
		
	}
}

void AR6PlayerCharacter::FireTimerHandle()
{
	bCanShoot = true;
}

void AR6PlayerCharacter::OnReload()
{
	bCanShoot = false;
	if (bAKVisibility == true)
	{
		if (bCanShoot == false)
		{
			if (PrimaryCurrentTotalAmmo > 0 && PrimaryCurrentMagAmmo != PrimaryMagMaxAmmo)
			{
				/*
				* if (ReloadAnimation)
				* {
				* WeaponMesh->PlayAnimation(ReloadAnimation, false);
				* }
				*/
				//UE_LOG(LogTemp, Warning, TEXT("CurrentTotalAmmo: %d"), CurrentTotalAmmo);
				int PrimaryDifference = PrimaryMagMaxAmmo - PrimaryCurrentMagAmmo;
				if (PrimaryCurrentTotalAmmo - PrimaryDifference >= 0)
				{
					PrimaryCurrentTotalAmmo -= PrimaryDifference;
					PrimaryCurrentMagAmmo = PrimaryMagMaxAmmo;
				}
				else
				{
					PrimaryCurrentMagAmmo += PrimaryCurrentTotalAmmo;
					PrimaryCurrentTotalAmmo = 0;
				}
			}

			GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &AR6PlayerCharacter::FireTimerHandle, ReloadTime, false);
		}
	}
	if (!bAKVisibility)
	{
		if (bCanShoot == false)
		{
			if (SecondaryCurrentTotalAmmo > 0 && SecondaryCurrentMagAmmo != SecondaryMagMaxAmmo)
			{
				/*
				* if (ReloadAnimation)
				* {
				* WeaponMesh->PlayAnimation(ReloadAnimation, false);
				* }
				*/
				//UE_LOG(LogTemp, Warning, TEXT("CurrentTotalAmmo: %d"), CurrentTotalAmmo);
				int SecondaryDifference = SecondaryMagMaxAmmo - SecondaryCurrentMagAmmo;
				if (SecondaryCurrentTotalAmmo - SecondaryDifference >= 0)
				{
					SecondaryCurrentTotalAmmo -= SecondaryDifference;
					SecondaryCurrentMagAmmo = SecondaryMagMaxAmmo;
				}
				else
				{
					SecondaryCurrentMagAmmo += SecondaryCurrentTotalAmmo;
					SecondaryCurrentTotalAmmo = 0;
				}
			}

			GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &AR6PlayerCharacter::FireTimerHandle, ReloadTime, false);
		}
	}
}


void AR6PlayerCharacter::Reload()
{

	//return Super::Reload(); here, and paste this function to the weapon's cpp
	//virtual override needs there!
	//if we made the weapon's class
	
}



void AR6PlayerCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void AR6PlayerCharacter::Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

}

void AR6PlayerCharacter::StartCrouch()
{
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	if (!GetCharacterMovement()->IsCrouching())
	{
		Crouch();


	}
	else
	{
		UnCrouch();
	}
}

void AR6PlayerCharacter::Vault()
{

	FHitResult OutHit;

	FVector Start = Ak47_SK->GetComponentLocation();
	FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector End = VaultEndPoint->GetComponentLocation(); 

	FCollisionQueryParams CollisionParams;

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1, 0, 1);

	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

	if (isHit)
	{
		if (OutHit.bBlockingHit)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("we can vault %s"),
					*OutHit.GetActor()->GetName()));
			}
		}
	}

}

void AR6PlayerCharacter::WeaponToPrimary()
{
		bAKVisibility = true;
		bPistolVisibility = false;

		Pistol_SK->SetVisibility(bPistolVisibility);
		Ak47_SK->SetVisibility(bAKVisibility);
	
}
void AR6PlayerCharacter::WeaponToSecondary()
{
		bAKVisibility = false;
		bPistolVisibility = true;

		Pistol_SK->SetVisibility(bPistolVisibility);
		Ak47_SK->SetVisibility(bAKVisibility);
		
		
}





