// Fill out your copyright notice in the Description page of Project Settings.


#include "BossTower.h"

// Sets default values
ABossTower::ABossTower()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABossTower::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossTower::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

