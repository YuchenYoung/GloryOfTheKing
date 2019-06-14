// Fill out your copyright notice in the Description page of Project Settings.


#include "BossTower.h"
#include <map>
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "testingCharacter.h"
#include "TinyHero.h"
#include "TowerActor.h"
#include "GameFramework/DamageType.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"

// Sets default values
ABossTower::ABossTower(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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
	TowerHealth->SetIsReplicated(true);
	MyBloodBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("MyBloodBar"));
	MyBloodBar->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	UClass* Widget = LoadClass<UUserWidget>(NULL, TEXT("WidgetBlueprint'/Game/TopDownCPP/Blueprints/WBP_TowerHealth.WBP_TowerHealth_C'"));
	MyBloodBar->SetWidgetClass(Widget);
	MyBloodBar->SetIsReplicated(true);
	
	fCauseDamage = 0.01f;
	bIsAttacking = false;
	bruined = false;
	bInMySide = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ABossTower::BeginPlay()
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

void ABossTower::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, AttackEffects, GetActorLocation());
}

void ABossTower::Collapse()
{
	if (Role < ROLE_Authority)
	{
		ServerCollapse();
	}
	
	// CollapseEffect();
	Meshcomp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Capsulecomp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackCapsulecomp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bruined = true;
	Destroy();
}

void ABossTower::ServerCollapse_Implementation()
{
	Collapse();
}
bool ABossTower::ServerCollapse_Validate()
{
	return true;
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

void ABossTower::NotifyActorEndOverlap(AActor* OtherActor)
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

bool ABossTower::GetInjured(AActor* DamageSource, float fDamageval)
{
	if (Role < ROLE_Authority)
	{
		ServerGetInjured(DamageSource, fDamageval);
	}
	
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

	TowerHealth->Damage(fDamageval, 1);
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

void ABossTower::ServerGetInjured_Implementation(AActor * DamageSource, float fDamageval)
{
	GetInjured(DamageSource, fDamageval);
}
bool ABossTower::ServerGetInjured_Validate(AActor * DamageSource, float fDamageval)
{
	return true;
}
