// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Enemyhero.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "MyHealthComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Perception/PawnSensingComponent.h"
#include <map>
#include "TinyHero.h"
#include "testingCharacter.h"
#include "TinyHero.h"
#include "TowerActor.h"
#include "BossTower.h"

using namespace std;

constexpr auto TIME_GOBACK = 2500;
constexpr auto TIME_SKILL = 1500;
constexpr auto TIME_RESTART = 2000;

AEnemyhero::AEnemyhero()
{
	AttackCapsulecomp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackingCapsulecomp"));
	AttackCapsulecomp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackCapsulecomp->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackCapsulecomp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AttackCapsulecomp->SetupAttachment(RootComponent);
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	PawnSensingComp->OnHearNoise.AddDynamic(this, &AEnemyhero::OnNoiseHeard);
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;

	// Configure character movement
	// Rotate character to moving direction


	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Create MyHealthComponent Class
	HeroHealth = CreateDefaultSubobject<UMyHealthComponent>(TEXT("HeroHealth"));


	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	NoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitter"));

	bInMySide = false;
	bDied = false;

	//set default money value
	Money = 10000;
	//set default attribution
	AttackValue = 20.0f;
	Defense = 1.0f;
	Energy = 10.0f;
	aEnergy = 0.01f;
	Level = 1;
	dLevel = 0.0f;
	aLevel = 0.3f;
	RestartTime = 0;
	Skill1Time = 0;
	Skill2Time = 0;
	Skill3Time = 0;
	for (int i = 0; i < 15; i++)
	{
		vLevelLib.push_back(0.20f - 0.01 * i);
	}
}

void AEnemyhero::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (RestartTime > 0)
	{
		if (RestartTime < TIME_RESTART)
		{
			RestartTime++;
		}
		else
		{
			SetActorLocation(FVector(1800, 1800, 243));
			RestartTime = 0;
			HeroHealth->InitialHealth();
			SetActorHiddenInGame(false);
		}
	}

	if (Skill1Time > 0)
	{
		if (Skill1Time < 150)
		{
			Skill1Time++;
		}
		else
		{
			Skill1Time = 0;
		}
	}

	if (Skill2Time > 0)
	{
		if (Skill2Time < 150)
		{
			Skill2Time++;
		}
		else
		{
			Skill2Time = 0;
		}
	}

	if (Skill3Time > 0)
	{
		if (Skill3Time < 150)
		{
			Skill3Time++;
		}
		else
		{
			Skill3Time = 0;
		}
	}
	/*
	if (CursorToWorld != nullptr)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UWorld * World = GetWorld())
			{
				FHitResult HitResult;
				FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
				FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
				FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if (APlayerController * PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
	*/
	//UMyHealthComponent Myhealth;
	//Myhealth.Damage(3);
	if (Energy < 100.0f)
	{
		Energy += aEnergy;
		if (Energy > 100.0f)
		{
			Energy = 100.0f;
		}
	}
	if (dLevel < 100.0f)
	{
		dLevel += aLevel;
		if (dLevel >= 100.0f)
		{
			dLevel -= 100.0f;
			aLevel *= 0.8f;
			Level++;
			if (Level > 15) {
				Level = 15;
			}
			else {
				OnLevelChanged();
			}
		}
	}
	if (bEffect3)
	{
		dEffect3 += fEffects3;
		if (dEffect3 < 30.0f)
		{
			HeroHealth->Health += fEffects3;
		}
		else
		{
			dEffect3 = 0.0f;
			bEffect3 = false;
		}
	}
}



bool AEnemyhero::GetInjured(AActor* DamageSource, float fDamageval)
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
	HeroHealth->Damage(fDamageval, 1);
	if (HeroHealth->JudgeDeath())
	{
		Die();
	}
	return true;
}

void AEnemyhero::OnLevelChanged()
{
	Defense *= Level * vLevelLib[Level - 2] + 1;
	aEnergy *= Level * vLevelLib[Level - 2] + 1;
	fDamageByEffect2 *= Level * vLevelLib[Level - 2] + 1;
	if (Level > 1)bCanEffect1 = true;
	if (Level > 2)
	{
		bCanEffect1 = true;
		bCanEffect2 = true;
	}
	if (Level > 3)
	{
		bCanEffect1 = true;
		bCanEffect2 = true;
		bCanEffect3 = true;
	}
}

void AEnemyhero::BeginPlay()
{
	Super::BeginPlay();
	direction = 1;
}

void AEnemyhero::Die()
{
	RestartTime = 1;
	bDied = true;
	SetActorHiddenInGame(true);
	//PlayDeathEffects();
	//Destroy();
}

void AEnemyhero::PlayDeathEffects()
{
	bDied = true;
	GetMovementComponent()->StopMovementImmediately();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyhero::PlayEffects1()
{
	
	if (Skill1Time == 0)
	{
		if (Energy < 10.0f || !bCanEffect1)return;
		UGameplayStatics::SpawnEmitterAtLocation(this, SkillEffectQ, GetActorLocation());
		Energy -= 10.0f;
		Skill1Time = 1;
	}

}

void AEnemyhero::PlayEffects2()
{
	
	if (Skill2Time == 0)
	{
		if (Energy < 10.0f || !bCanEffect2)return;
		UGameplayStatics::SpawnEmitterAtLocation(this, SkillEffectW, GetActorLocation());
		Energy -= 10.0f;
		Skill2Time = 1;
		if (bIsAttackByEffects)
		{
			map<AActor*, int>::iterator iToAttack;
			for (iToAttack = mWillAttack.begin(); iToAttack != mWillAttack.end(); iToAttack++)
			{
				AActor* ATemp = iToAttack->first;
				AtestingCharacter* InjuredHero = Cast<AtestingCharacter>(ATemp);
				if (InjuredHero)
				{
					InjuredHero->GetInjured(this, this->fDamageByEffect2);
				}
				else
				{
					ATowerActor* InjuredTower = Cast<ATowerActor>(ATemp);
					if (InjuredTower)
					{
						InjuredTower->GetInjured(this, this->fDamageByEffect2);
					}
					else
					{
						ABossTower* InjuredBoss = Cast<ABossTower>(ATemp);
						if (InjuredBoss)
						{
							InjuredBoss->GetInjured(this, this->fDamageByEffect2);
						}
					}
				}
			}
		}
	}
}

void AEnemyhero::PlayEffects3()
{
	
	if (Skill3Time == 0)
	{
		if (!bCanEffect3 || Energy < 30.0f)return;
		UGameplayStatics::SpawnEmitterAtLocation(this, SkillEffectE, GetActorLocation());

		bEffect3 = true;

		Energy -= 30.0f;
		Skill3Time = 1;
	}
}

void AEnemyhero::PlayEffects4()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, SkillEffectR, GetActorLocation());
}

void AEnemyhero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Keyboard1", IE_Pressed, this, &AEnemyhero::PlayEffects1);
	PlayerInputComponent->BindAction("Keyboard2", IE_Pressed, this, &AEnemyhero::PlayEffects2);
	PlayerInputComponent->BindAction("Keyboard3", IE_Pressed, this, &AEnemyhero::PlayEffects3);
	PlayerInputComponent->BindAction("Keyboard4", IE_Pressed, this, &AEnemyhero::PlayEffects4);

	PlayerInputComponent->BindAxis("MoveForward", this, &AEnemyhero::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AEnemyhero::MoveRight);

}

void AEnemyhero::MoveForward(float val)
{
	AddMovementInput(GetActorForwardVector() * val);
}

void AEnemyhero::MoveRight(float val)
{
	AddMovementInput(GetActorRightVector() * val);
}




void AEnemyhero::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{

	//DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);
	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();
	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;
	SetActorRotation(NewLookAt);
	PlayEffects2();
	PlayEffects3();
	PlayEffects1();
}



