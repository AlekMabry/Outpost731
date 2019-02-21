// Fill out your copyright notice in the Description page of Project Settings.

#include "player_pc.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Engine/EngineTypes.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/GameEngine.h"

// Sets default values
Aplayer_pc::Aplayer_pc()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorEnableCollision(true);

	// Set size for collision capsule
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Capsule"));
	RootComponent = CapsuleComponent;
	CapsuleComponent->SetCollisionProfileName(TEXT("Player Collision"));
	CapsuleComponent->InitCapsuleSize(cap_radius, caphalf_standing_height);
	CapsuleComponent->SetSimulatePhysics(true);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComponent->BodyInstance.bLockXRotation = true;
	CapsuleComponent->BodyInstance.bLockYRotation = true;
	CapsuleComponent->BodyInstance.SetDOFLock(EDOFMode::Default);

	if (Capsule_PhysicsMaterial)
	{
		CapsuleComponent->SetPhysMaterialOverride(Capsule_PhysicsMaterial);
	}

	CapsuleComponent->WakeRigidBody();

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(CapsuleComponent);
	FirstPersonCameraComponent->RelativeLocation = camera_offset; // FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void Aplayer_pc::BeginPlay()
{
	Super::BeginPlay();
	
}

void Aplayer_pc::eventInputForward(float Val)
{
	inputForward = Val;
}

void Aplayer_pc::eventInputRight(float Val)
{
	inputRight = Val;
}

void Aplayer_pc::eventActivateSprint()
{
	inputSprint = true;
}

void Aplayer_pc::eventReleaseSprint()
{
	inputSprint = false;
}

void Aplayer_pc::eventActivateCrouch()
{
	inputCrouch = true;
}

void Aplayer_pc::eventReleaseCrouch()
{
	inputCrouch = false;
}

void Aplayer_pc::eventActivateJump()
{
	inputJump = true;
}

void Aplayer_pc::eventReleaseJump()
{
	inputJump = false;
}

FVector Aplayer_pc::Accelerate(FVector accelDir, float deltaTime)
{
	float projVel = FVector::DotProduct(GetVelocity(), accelDir); // Vector projection of Current velocity onto accelDir.
	float accelVel = sv_accelerate * deltaTime; // Accelerated velocity in direction of movment

	// If necessary, truncate the accelerated velocity so the vector projection does not exceed max_velocity
	if (projVel + accelVel > sv_max_velocity)
		accelVel = sv_max_velocity - projVel;

	return accelDir * (accelVel/deltaTime) * 100;
}

// Called every frame
void Aplayer_pc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult GroundHitResult(ForceInit);
	FCollisionQueryParams GroundTraceParams = FCollisionQueryParams(FName(TEXT("GroundTrace")), true, this);;
	GroundTraceParams.bTraceComplex = true;
	GroundTraceParams.bTraceAsyncScene = false;
	GroundTraceParams.bReturnPhysicalMaterial = false;
	FTransform CapsuleTransform = CapsuleComponent->GetComponentTransform();
	FVector TraceStartPosition = CapsuleTransform.GetLocation();
	TraceStartPosition -= FVector(0.0f, 0.0f, CapsuleComponent->GetScaledCapsuleHalfHeight());
	FVector TraceEndPosition = TraceStartPosition - FVector(0.0f, 0.0f, 5.0f);
	stateOnGround = GetWorld()->LineTraceSingleByChannel(GroundHitResult, TraceStartPosition, TraceEndPosition, ECC_WorldStatic, GroundTraceParams);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Touching Ground: %d, Input: %d"), stateOnGround, inputJump));
	}

	// Change dimensions of capsule to simulate crouching and standing
	if (inputCrouch)
	{
		// Decrease capsule height to crouching height
		float rad;
		float halfheight;
		CapsuleComponent->GetUnscaledCapsuleSize(rad, halfheight);
		if (halfheight > caphalf_crouching_height)
		{
			float sizeDifference = cap_crouchRate * DeltaTime;
			if (halfheight - sizeDifference < caphalf_crouching_height)
			{
				// The capsule height has decreased below crouching height, so cap it to crouching height
				CapsuleComponent->SetCapsuleHalfHeight(caphalf_crouching_height, true);
			}
			else
			{
				// Decrease height of capsule to simulate crouching
				CapsuleComponent->SetCapsuleHalfHeight(halfheight - sizeDifference, true);
			}
			// Update camera position, camera should always be a specific height from top of collision capsule.
			FirstPersonCameraComponent->RelativeLocation = FVector(camera_offset.X, camera_offset.Y, CapsuleComponent->GetUnscaledCapsuleHalfHeight() - camera_eyeToTopOffset);
		}
	}
	else
	{
		// Increase capsule height to standing height
		float rad;
		float halfheight;
		CapsuleComponent->GetUnscaledCapsuleSize(rad, halfheight);
		if (halfheight < caphalf_standing_height)
		{
			float sizeDifference = cap_crouchRate * DeltaTime;
			if (halfheight + sizeDifference > caphalf_standing_height)
			{
				// The capsule height has decreased below crouching height, so cap it to crouching height
				CapsuleComponent->SetCapsuleHalfHeight(caphalf_standing_height, true);
			}
			else
			{
				// Decrease height of capsule to simulate crouching
				CapsuleComponent->SetCapsuleHalfHeight(halfheight + sizeDifference, true);
			}

			// Update camera position, camera should always be a specific height from top of collision capsule.
			FirstPersonCameraComponent->RelativeLocation = FVector(camera_offset.X, camera_offset.Y, CapsuleComponent->GetUnscaledCapsuleHalfHeight() - camera_eyeToTopOffset);
		}
	}

	FVector AccelerationForward;
	if (stateOnGround)
	{
		AccelerationForward = Accelerate(FirstPersonCameraComponent->GetForwardVector()*inputForward, DeltaTime);
	}
	else
	{
		AccelerationForward = 0.05*Accelerate(FirstPersonCameraComponent->GetForwardVector()*inputForward, DeltaTime);
	}
	CapsuleComponent->AddForce(AccelerationForward);

	FVector AccelerationRight;
	if (stateOnGround)
	{
		AccelerationRight = Accelerate(FirstPersonCameraComponent->GetRightVector()*inputRight, DeltaTime);
	}
	else
	{
		AccelerationRight = 0.05*Accelerate(FirstPersonCameraComponent->GetRightVector()*inputRight, DeltaTime);
	}
	CapsuleComponent->AddForce(AccelerationRight);

	if (inputJump == true)
	{
		if (stateOnGround == true)
		{
			CapsuleComponent->AddForce(FVector(0.0f, 0.0f, 5000000.0f));
		}
	}
}

// Called to bind functionality to input
void Aplayer_pc::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind fire event
	// PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AOutpost731Character::OnFire);

	// PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AOutpost731Character::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &Aplayer_pc::eventInputForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &Aplayer_pc::eventInputRight);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &Aplayer_pc::eventActivateSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &Aplayer_pc::eventReleaseSprint);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &Aplayer_pc::eventActivateCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &Aplayer_pc::eventReleaseCrouch);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &Aplayer_pc::eventActivateJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &Aplayer_pc::eventReleaseJump);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	// PlayerInputComponent->BindAxis("TurnRate", this, &AOutpost731Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	// PlayerInputComponent->BindAxis("LookUpRate", this, &AOutpost731Character::LookUpAtRate);
}

