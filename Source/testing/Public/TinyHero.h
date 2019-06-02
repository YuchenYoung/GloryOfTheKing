// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TinyHero.generated.h"

using namespace UP;
using namespace UI;

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

protected:
	void Die();
};
