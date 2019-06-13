// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BossTower.generated.h"

using namespace UP;
using namespace std;

class UCapsuleComponent;
class UMyHealthComponent;

UCLASS()
class TESTING_API ABossTower : public APawn
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Var")
	bool bruined;
	bool bIsAttacking;
	float fCauseDamage;
	map<AActor*, int> mWillAttack;

public:
	UPROPERTY(EditInstanceOnly, Category = "Side")
	bool bInMySide;

public:
	// Sets default values for this pawn's properties
	ABossTower();

protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* Meshcomp;
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCapsuleComponent* Capsulecomp;
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCapsuleComponent* AttackCapsulecomp;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* AttackEffects;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	UMyHealthComponent* TowerHealth;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlayEffects();

	void Collapse();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	UFUNCTION(BlueprintCallable)
	bool GetInjured(AActor* DamageSource, float fDamageval);

};
