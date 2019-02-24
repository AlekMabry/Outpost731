// Fill out your copyright notice in the Description page of Project Settings.

#include "npc_ArcticWolf.h"

// Sets default values
Anpc_ArcticWolf::Anpc_ArcticWolf()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Anpc_ArcticWolf::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Anpc_ArcticWolf::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void Anpc_ArcticWolf::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

