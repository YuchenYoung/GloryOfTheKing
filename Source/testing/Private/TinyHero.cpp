// Fill out your copyright notice in the Description page of Project Settings.


#include "TinyHero.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include"testingGameMode.h"
#include "MyHealthComponent.h"
#include "Perception/PawnSensingComponent.h"
#include"DrawDebugHelpers.h"
#include"AI/NavigationSystemBase.h"
#include"Net/UnrealNetwork.h"

// Sets default values
ATinyHero::ATinyHero()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ATinyHero::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &ATinyHero::OnNoiseHeard);

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
	if (bPatrol)
	{
		MoveToNextPatrolPoint();
	}
}
void ATinyHero::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr)
		return;
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);
}


void ATinyHero::OnNoiseHeard(APawn * NoiseInstigator, const FVector & Location, float Volume)
{

	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);
	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();
	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;
	SetActorRotation(NewLookAt);

	AController* Controller = GetController();
	if (Controller)
	{
		Controller->StopMovement();
	}
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

void ATinyHero::MoveToNextPatrolPoint()
{
	if (CurrentPatrolPoint == nullptr || CurrentPatrolPoint == SecondPatrolPoint)
	{
		CurrentPatrolPoint = FirstPatrolPoint;
	}
	else
	{
		CurrentPatrolPoint = SecondPatrolPoint;
	}
	UNavigationSystem::SimpleMoveToActor(GetController(), CurrentPatrolPoint);

}




