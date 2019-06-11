// Fill out your copyright notice in the Description page of Project Settings.


#include "TinyHero.h"
#include <map>
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
//#include "testingGameMode.h"
#include "MyGameModeBase.h"
#include "MyHealthComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "AI/NavigationSystemBase.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Kismet/GameplayStatics.h"
#include "testingCharacter.h"
#include "TowerActor.h"
#include "Net/UnrealNetwork.h"

using namespace std;

// Sets default values
ATinyHero::ATinyHero()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ATinyHero::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &ATinyHero::OnNoiseHeard);

	TinyHealth = CreateDefaultSubobject<UMyHealthComponent>(TEXT("TinyHealth"));

	fCauseDamage = 0.5;
	bIsAttacking = false;
}

// Called when the game starts or when spawned
void ATinyHero::BeginPlay()
{
	Super::BeginPlay();
	/*
	if (bPatrol)
	{
		MoveToNextPatrolPoint();
	}
<<<<<<< HEAD
	*/

}

void ATinyHero::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr) return;
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


	/*AController* Controller = GetController();
	if (Controller)
	{
		Controller->StopMovement();
	}*/

	/*
	AController* Controller = GetController();
	if (Controller)
	{
		Controller->StopMovement();
	}
	*/
}
void ATinyHero::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, AttackEffects, GetActorLocation());

}
// Called every frame
void ATinyHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAttacking)
	{
		PlayEffects();
		map<AActor*, int>::iterator iToAttack;
		for (iToAttack = mWillAttack.begin(); iToAttack != mWillAttack.end(); iToAttack++)
		{
			AActor* ATemp = iToAttack->first;
			AtestingCharacter* InjuredHero = Cast<AtestingCharacter>(ATemp);
			if (InjuredHero)
			{
				InjuredHero->GetInjured(this, this->fCauseDamage);
			}
			else
			{
				ATowerActor* InjuredTower = Cast<ATowerActor>(ATemp);
				if (InjuredTower)
				{
					InjuredTower->GetInjured(this, this->fCauseDamage);
				}
			}
		}
	}
}

// Called to bind functionality to input
void ATinyHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	
}

void ATinyHero::GetInjured(AActor* DamageSource, float fDamageval)
{
	TinyHealth->Damage(fDamageval,1);
	if (TinyHealth->JudgeDeath())
	{
		Die();
	}
}

void ATinyHero::Die()
{
	//PlayDeathEffect();
	bDied = true;
	//Destroy();
}

void ATinyHero::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (mWillAttack.find(OtherActor) == mWillAttack.end())
	{
		mWillAttack.insert(pair<AActor*, int>(OtherActor, 1));
	}
	bIsAttacking = true;
}

void ATinyHero::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	mWillAttack.erase(OtherActor);
	if (mWillAttack.empty())
	{
		bIsAttacking = false;
	}
}

/*
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
*/



