// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerActor.h"
#include <map>
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "testingCharacter.h"
#include "TinyHero.h"
#include "BossTower.h"
#include "Enemyhero.h"
#include "GameFramework/DamageType.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"

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
	MyBloodBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("MyBloodBar"));
	MyBloodBar->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	UClass* Widget = LoadClass<UUserWidget>(NULL, TEXT("WidgetBlueprint'/Game/TopDownCPP/Blueprints/WBP_TowerHealth.WBP_TowerHealth_C'"));
	MyBloodBar->SetWidgetClass(Widget);
	MyBloodBar->SetIsReplicated(true);

	TowerHealth->Health = 200.0f;
	fCauseDamage = 0.005f;
	bIsAttacking = false;
	bruined = false;
	bisfiring = false;
	bInMySide = true;
}

// Called when the game starts or when spawned
void ATowerActor::BeginPlay()
{
	Super::BeginPlay();
	
	UUserWidget* CurrentWidget = MyBloodBar->GetUserWidgetObject();
	if (CurrentWidget != NULL)
	{
		HPBarProgress = Cast<UProgressBar>(CurrentWidget->GetWidgetFromName(TEXT("TowerBar")));

		if (HPBarProgress != NULL)
		{
			HPBarProgress->SetPercent(1.0f);
		}
	}

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
	AEnemyhero* OtherAI = Cast<AEnemyhero>(DamageSource);
	if (OtherAI && OtherAI->bInMySide == this->bInMySide)
	{
		return false;
	}
	TowerHealth->Damage(fDamageval,1);
	if (TowerHealth->JudgeDeath())
	{
		Collapse();
	}
	if (HPBarProgress != NULL)
	{
		HPBarProgress->SetPercent(TowerHealth->HealthLeft());
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

void ATowerActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	float Distance = (this->GetActorLocation() - OtherActor->GetActorLocation()).Size();
	if (Distance > fDamageRadius) return;
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

