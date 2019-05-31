// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHealthComponent.h"

// Sets default values for this component's properties
UMyHealthComponent::UMyHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	Health = 100;
}


// Called when the game starts
void UMyHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UMyHealthComponent::Damage(float Damageval)
{
	Health -= Damageval;
}

bool UMyHealthComponent::JudgeDeath()
{
	return Health <= 0;
}
