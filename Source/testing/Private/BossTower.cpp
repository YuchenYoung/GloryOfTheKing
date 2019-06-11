// Fill out your copyright notice in the Description page of Project Settings.


#include "BossTower.h"
#include <map>
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "testingCharacter.h"
#include "TinyHero.h"
#include "GameFramework/DamageType.h"

// Sets default values
ABossTower::ABossTower()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Meshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Meshcomp"));
	RootComponent = Cast<USceneComponent>(Meshcomp);
	Capsulecomp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsulecomp"));
	Capsulecomp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Capsulecomp->SetCollisionResponseToAllChannels(ECR_Ignore);
	Capsulecomp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Capsulecomp->SetupAttachment(RootComponent);
	AttackCapsulecomp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackingCapsulecomp"));
	AttackCapsulecomp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackCapsulecomp->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackCapsulecomp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AttackCapsulecomp->SetupAttachment(RootComponent);
	TowerHealth = CreateDefaultSubobject<UMyHealthComponent>(TEXT("TowerHealth"));

	fCauseDamage = 0.05;
	bIsAttacking = false;
	bruined = false;

}

// Called when the game starts or when spawned
void ABossTower::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABossTower::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, AttackEffects, GetActorLocation());
}

void ABossTower::Collapse()
{
	// CollapseEffect();
	Meshcomp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Capsulecomp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackCapsulecomp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bruined = true;
	Destroy();
}

// Called every frame
void ABossTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bruined)
	{
		return;
	}
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
				//InjuredHero->HeroHealth->HandleTakeAnyDamage(InjuredHero, fCauseDamage, InjuredHero->HeroHealth->DefaultDamageType, InjuredHero->HeroHealth->DefaultConTroller, this);
			}
			else
			{
				ATinyHero* InjuredTiny = Cast<ATinyHero>(ATemp);
				if (InjuredTiny)
				{
					InjuredTiny->GetInjured(this, this->fCauseDamage);
				}
			}
		}
	}
}

void ABossTower::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (mWillAttack.find(OtherActor) == mWillAttack.end())
	{
		mWillAttack.insert(pair<AActor*, int>(OtherActor, 1));
	}
	bIsAttacking = true;
}

void ABossTower::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	mWillAttack.erase(OtherActor);
	if (mWillAttack.empty())
	{
		bIsAttacking = false;
	}
}

void ABossTower::GetInjured(AActor* DamageSource, float fDamageval)
{
	TowerHealth->Damage(fDamageval, 1);
	if (TowerHealth->JudgeDeath())
	{
		Collapse();
	}
}

