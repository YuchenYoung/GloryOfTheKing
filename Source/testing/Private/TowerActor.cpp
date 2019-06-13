// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerActor.h"
#include <map>
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "testingCharacter.h"
#include "TinyHero.h"
#include "BossTower.h"
#include "GameFramework/DamageType.h"

// Sets default values
ATowerActor::ATowerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

	fCauseDamage = 0.005;
	bIsAttacking = false;
	bruined = false;
	bisfiring = false;
	bInMySide = true;
}

// Called when the game starts or when spawned
void ATowerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATowerActor::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, AttackEffects, GetActorLocation());
}

void ATowerActor::PlayCollapseEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, CollapseEffects, GetActorLocation());
}

bool ATowerActor::GetInjured(AActor* DamageSource, float fDamageval)
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

	TowerHealth->Damage(fDamageval,1);
	if (TowerHealth->JudgeDeath())
	{
		Collapse();
	}
	return true;
}

void ATowerActor::Collapse()
{
	// CollapseEffect();
	Meshcomp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Capsulecomp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackCapsulecomp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Meshcomp->SetMaterial(0, RuinMaterial);
	bruined = true;
	//PlayCollapseEffects();
	Destroy();
}

// Called every frame
void ATowerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bruined && bisfiring)
	{
		PlayCollapseEffects();
		return;
	}
	if (bruined)
	{
		bisfiring = true;
		PlayCollapseEffects();
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

void ATowerActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (mWillAttack.find(OtherActor) == mWillAttack.end())
	{
		AtestingCharacter* OtherHero = Cast<AtestingCharacter>(OtherActor);
		if (OtherHero && OtherHero->bInMySide != this->bInMySide)
		{
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
	}
	
}

void ATowerActor::NotifyActorEndOverlap(AActor* OtherActor)
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

