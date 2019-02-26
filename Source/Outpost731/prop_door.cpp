// Fill out your copyright notice in the Description page of Project Settings.

#include "prop_door.h"

// Sets default values
Aprop_door::Aprop_door()
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("<name>"));
	//MeshComponent->SetStaticMesh(<your mesh as UStaticMesh*>);
	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &Aprop_door::playerOverlap);


}

// Called when the game starts or when spawned
void Aprop_door::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Aprop_door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

