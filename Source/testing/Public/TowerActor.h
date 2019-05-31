// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerActor.generated.h"

using namespace UP;
using namespace std;

class UCapsuleComponent;
class UMyHealthComponent;

UCLASS()
class TESTING_API ATowerActor : public AActor
{
	GENERATED_BODY()
	
protected:

	bool bIsAttacking;
	float fCauseDamage;
	map<AActor*, int> mWillAttack;

public:	
	// Sets default values for this actor's properties
	ATowerActor();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* Meshcomp;
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCapsuleComponent* Capsulecomp;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* AttackEffects;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	UMyHealthComponent* TowerHealth;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlayEffects();

	void GetInjured(AActor* DamageSource, float fDamageval);

	void Collapse();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
};
