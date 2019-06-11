// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHealthComponent.h"
#include"GameFramework/DamageType.h"
#include"GameFramework/Controller.h"
#include"TinyHero.h"

// Sets default values for this component's properties
UMyHealthComponent::UMyHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	//DefaultDamageType = CreateDefaultSubobject<UDamageType>(TEXT("DefaultDamageType"));
	//DefaultConTroller = CreateDefaultSubobject<AController>(TEXT("DefaultConTroller"));

	DefaultHealth = 100;
}


// Called when the game starts
void UMyHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UMyHealthComponent::HandleTakeAnyDamage);
	}
	 Health = DefaultHealth;
}

void UMyHealthComponent::Damage(float Damageval,float Defense)
{
	Health -= Defense * Damageval;
	if (Health < 0) Health = 0;
}

void UMyHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f)
	{
		return;
	}

	//Update health clamped
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	UE_LOG(LogTemp, Log, TEXT("Health Changed:%s"), *FString::SanitizeFloat(Health));

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType,InstigatedBy, DamageCauser);
}

bool UMyHealthComponent::JudgeDeath()
{
	return Health <= 0;
}

