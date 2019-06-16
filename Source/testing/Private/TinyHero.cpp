// Fill out your copyright notice in the Description page of Project Settings.


#include "TinyHero.h"
#include <map>
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyGameModeBase.h"
#include "MyHealthComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Kismet/GameplayStatics.h"
#include "testingCharacter.h"
#include "TowerActor.h"
#include "BossTower.h"
#include "Enemyhero.h"


using namespace std;

// Sets default values
ATinyHero::ATinyHero()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	OuterCapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("OuterCapsuleComp"));
	OuterCapsuleComp->SetupAttachment(RootComponent);

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ATinyHero::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &ATinyHero::OnNoiseHeard);
	TinyHealth = CreateDefaultSubobject<UMyHealthComponent>(TEXT("TinyHealth"));

	

	fCauseDamage = 0.05f;
	bIsAttacking = false;
	bMoveToTower = true;

}

// Called when the game starts or when spawned
void ATinyHero::BeginPlay()
{
	Super::BeginPlay();

}

void ATinyHero::SetDefaultSide(bool bSide)
{
	bInMySide = bSide;
}

void ATinyHero::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr) return;
}


void ATinyHero::OnNoiseHeard(APawn * NoiseInstigator, const FVector & Location, float Volume)
{

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();
	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;
	SetActorRotation(NewLookAt);


	
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
				else
				{
					ABossTower* InjuredBoss = Cast<ABossTower>(ATemp);
					if (InjuredBoss)
					{
						InjuredBoss->GetInjured(this, this->fCauseDamage);
					}
					else
					{
						AEnemyhero* InjuredAI = Cast<AEnemyhero>(ATemp);
						if (InjuredAI)
						{
							InjuredAI->GetInjured(this, this->fCauseDamage);
						}
					}
				}
			}
		}
	}
}


bool ATinyHero::GetInjured(AActor* DamageSource, float fDamageval)
{
	AtestingCharacter* OtherHero = Cast<AtestingCharacter>(DamageSource);
	if (OtherHero && OtherHero->bInMySide == this->bInMySide)
	{
		return false;
	}
	ATinyHero* OtherTiny = Cast<ATinyHero>(DamageSource);
	if (OtherTiny && OtherTiny->bInMySide == this->bInMySide)
	{
		return false;
	}
	ATowerActor* OtherTower = Cast<ATowerActor>(DamageSource);
	if (OtherTower && OtherTower->bInMySide == this->bInMySide)
	{
		return false;
	}
	ABossTower* OtherBoss = Cast<ABossTower>(DamageSource);
	if (OtherBoss && OtherBoss->bInMySide == this->bInMySide)
	{
		return false;
	}
	AEnemyhero* OtherAI = Cast<AEnemyhero>(DamageSource);
	if (OtherAI && OtherAI->bInMySide == this->bInMySide)
	{
		return false;
	}
	TinyHealth->Damage(fDamageval, 1);
	if (TinyHealth->JudgeDeath())
	{
		AtestingCharacter* SourceHero = Cast<AtestingCharacter>(DamageSource);
		if (SourceHero)
		{
			SourceHero->AddResult_Tiny();
		}
		Die();
	}
	return true;
}

void ATinyHero::Die()
{
	//PlayDeathEffect();
	bDied = true;
	if (Role == ROLE_Authority)
	{
		Destroy();
	}
}

void ATinyHero::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (mWillAttack.find(OtherActor) == mWillAttack.end())
	{
		AtestingCharacter* OtherHero = Cast<AtestingCharacter>(OtherActor);
		if (OtherHero && OtherHero->bInMySide != this->bInMySide)
		{
			float Distance = (this->GetActorLocation() - OtherActor->GetActorLocation()).Size();
			if (Distance > fDamageRadius) return;
			mWillAttack.insert(pair<AActor*, int>(OtherActor, 1));
			bIsAttacking = true;
			return;
		}
		ATinyHero* OtherTiny = Cast<ATinyHero>(OtherActor);
		if (OtherTiny && OtherTiny->bInMySide != this->bInMySide)
		{
			mWillAttack.insert(pair<AActor*, int>(OtherActor, 1));
			bIsAttacking = true;
			return;
		}
		ATowerActor* OtherTower = Cast<ATowerActor>(OtherActor);
		if (OtherTower && OtherTower->bInMySide != this->bInMySide)
		{
			mWillAttack.insert(pair<AActor*, int>(OtherActor, 1));
			bIsAttacking = true;
			return;
		}
		ABossTower* OtherBoss = Cast<ABossTower>(OtherActor);
		if (OtherBoss && OtherBoss->bInMySide != this->bInMySide)
		{
			mWillAttack.insert(pair<AActor*, int>(OtherActor, 1));
			bIsAttacking = true;
			return;
		}
	}
}

void ATinyHero::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if (mWillAttack.find(OtherActor) == mWillAttack.end())
	{
		return;
	}
	mWillAttack.erase(OtherActor);
	if (mWillAttack.empty())
	{
		bIsAttacking = false;
	}
}

