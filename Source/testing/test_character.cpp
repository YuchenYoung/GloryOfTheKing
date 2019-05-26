// Fill out your copyright notice in the Description page of Project Settings.


#include "test_character.h"

// Sets default values for this component's properties
Utest_character::Utest_character()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void Utest_character::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void Utest_character::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

