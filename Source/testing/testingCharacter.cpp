// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "testingCharacter.h"
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
#include "TinyHero.h"
#include "TowerActor.h"
#include "BossTower.h"
#include "Components/PawnNoiseEmitterComponent.h"

#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Components/CapsuleComponent.h"


	





AtestingCharacter::AtestingCharacter()
{
	AttackCapsulecomp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackingCapsulecomp"));
	AttackCapsulecomp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackCapsulecomp->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackCapsulecomp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AttackCapsulecomp->SetupAttachment(RootComponent);
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

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
	
	// Set side
	bInMySide = 1;

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
}

void AtestingCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UWorld* World = GetWorld())
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
		
		/*
		else if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
		*/

	}
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
			if (Level > 15) 
			{
				Level = 15;
			}
			else
			{
				OnLevelChanged();
			}
		}
	}
}

void AtestingCharacter::OnHealthChanged(UMyHealthComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bDied)
	{
		//Die!
		bDied = true;

		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

bool AtestingCharacter::GetInjured(AActor* DamageSource, float fDamageval)
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

	HeroHealth->Damage(fDamageval,Defense);

	if (HeroHealth->JudgeDeath()&&!bDied)
	{
		Die();
	}
	return true;
}

void AtestingCharacter::OnLevelChanged()
{
	Defense *= Level * 0.1 + 1;
	aEnergy *= Level * 0.1 + 1;
}

void AtestingCharacter::BeginPlay()
{
	Super::BeginPlay();

}
void AtestingCharacter::Die()
{
	PlayDeathEffects();
	//Destroy();
}
void AtestingCharacter::PlayDeathEffects()
{
	bDied = true;
	GetMovementComponent()->StopMovementImmediately();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AtestingCharacter::PlayEffectsQ()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, SkillEffectQ, GetActorLocation());
}

void AtestingCharacter::PlayEffectsW()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, SkillEffectW, GetActorLocation());
}

void AtestingCharacter::PlayEffectsE()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, SkillEffectE, GetActorLocation());
}

void AtestingCharacter::PlayEffectsR()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, SkillEffectR, GetActorLocation());
}

void AtestingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("KeyboardQ", IE_Pressed, this, &AtestingCharacter::PlayEffectsQ);
	PlayerInputComponent->BindAction("KeyboardW", IE_Pressed, this, &AtestingCharacter::PlayEffectsW);
	PlayerInputComponent->BindAction("KeyboardE", IE_Pressed, this, &AtestingCharacter::PlayEffectsE);
	PlayerInputComponent->BindAction("KeyboardR", IE_Pressed, this, &AtestingCharacter::PlayEffectsR);


}




