// Fill out your copyright notice in the Description page of Project Settings.


#include "TinyHero.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyHealthComponent.h"


// Sets default values
ATinyHero::ATinyHero()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Cast<USceneComponent>(Mesh);
	Capsulecomp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsulecomp"));
	Capsulecomp->SetupAttachment(RootComponent);
	*/
	TinyHealth = CreateDefaultSubobject<UMyHealthComponent>(TEXT("TinyHealth"));
}

// Called when the game starts or when spawned
void ATinyHero::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATinyHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATinyHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATinyHero::GetInjured(AActor* DamageSource, float fDamageval)
{
	TinyHealth->Damage(fDamageval);
	if (TinyHealth->JudgeDeath())
	{
		Die();
	}
}

void ATinyHero::Die()
{
	//PlayDeathEffect();
	Destroy();
}


