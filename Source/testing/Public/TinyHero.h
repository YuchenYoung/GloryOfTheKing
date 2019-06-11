// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TinyHero.generated.h"

using namespace UP;
using namespace UI;

class UPawnSensingComponent;
class UCapsuleComponent;
class UMyHealthComponent;

UCLASS()
class TESTING_API ATinyHero : public ACharacter
{
	GENERATED_BODY()

	
public:
	// Sets default values for this character's properties
	ATinyHero();

protected:
	UPROPERTY(EditInstanceOnly, Category = "AI")
		bool bPatrol;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol"))
		AActor * FirstPatrolPoint;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol"))
		AActor * SecondPatrolPoint;

	AActor* CurrentPatrolPoint;

	void MoveToNextPatrolPoint();

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UPawnSensingComponent* PawnSensingComp;
	UFUNCTION()
		void OnPawnSeen(APawn* SeenPawn);
	UFUNCTION()
		void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

	/*
	UPROPERTY(VisibleAnywhere, Category = "Component")
	USkeletalMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCapsuleComponent* Capsulecomp;
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healthcomp")
	UMyHealthComponent* TinyHealth;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void GetInjured(AActor* DamageSource, float fDamageval);
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	bool bDied;
protected:
	void Die();
};
