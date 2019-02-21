// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "player_pc.generated.h"

UCLASS()
class OUTPOST731_API Aplayer_pc : public APawn
{
	GENERATED_BODY()

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float sv_accelerate = 12800;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float sv_max_velocity = 800;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		bool movemech_wallrun = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		bool movemech_airstrafe = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		bool movemech_doublejump = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		bool movemech_crouchaccelerate = false;

	/** Character Dimensions **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float cap_radius = 40;		// Half height of capsule when standing

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float caphalf_standing_height = 90;		// Half height of capsule when standing

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float caphalf_crouching_height = 45;	// Half height of capsule when crouching

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float cap_crouchRate = 200;		// Centimeters per second

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float camera_eyeToTopOffset = 20;	// Centimeters from eyes to top of head

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		FVector camera_offset = FVector(0, 0, 70.0);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UPhysicalMaterial* Capsule_PhysicsMaterial;

public:
	// Sets default values for this pawn's properties
	Aplayer_pc();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float inputForward = 0;
	float inputRight = 0;
	bool inputCrouch = false;
	bool inputSprint = false;
	bool inputJump = false;
	bool stateOnGround = false;

	void eventInputForward(float Val);
	void eventInputRight(float Val);
	void eventActivateSprint();
	void eventReleaseSprint();
	void eventActivateCrouch();
	void eventReleaseCrouch();
	void eventActivateJump();
	void eventReleaseJump();
	FVector Accelerate(FVector accelDir, float deltaTime);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};
