// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerActor.h"
#include <map>
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "testingCharacter.h"

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
	TowerHealth = CreateDefaultSubobject<UMyHealthComponent>(TEXT("TowerHealth"));

	fCauseDamage = 0.8;
	bIsAttacking = false;
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

void ATowerActor::GetInjured(AActor* DamageSource, float fDamageval)
{
	TowerHealth->Damage(fDamageval);
	if (TowerHealth->JudgeDeath())
	{
		Collapse();
	}
}

void ATowerActor::Collapse()
{
	// CollapseEffect();
	Destroy();
}

// Called every frame
void ATowerActor::Tick(float DeltaTime)
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
		}
	}
}

void ATowerActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (mWillAttack[OtherActor] == 0)
	{
		mWillAttack[OtherActor] = 1;
	}
	bIsAttacking = true;
}

void ATowerActor::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	mWillAttack.erase(OtherActor);
	if (mWillAttack.empty())
	{
		bIsAttacking = false;
	}
}

