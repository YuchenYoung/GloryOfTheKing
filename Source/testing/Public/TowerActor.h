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
class UWidgetComponent;
class UProgressBar;

UCLASS()
class TESTING_API ATowerActor : public AActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "var")
	bool bruined;
	bool bisfiring;
	bool bIsAttacking;
	float fCauseDamage;
	map<AActor*, int> mWillAttack;

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Side")
	bool bInMySide;

public:	
	// Sets default values for this actor's properties
	ATowerActor();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* Meshcomp;
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCapsuleComponent* Capsulecomp;
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCapsuleComponent* AttackCapsulecomp;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* AttackEffects;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* CollapseEffects;
	UPROPERTY(EditDefaultsOnly, Category = "Material")
	UMaterialInterface* RuinMaterial;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	UMyHealthComponent* TowerHealth;
	UPROPERTY(EditAnywhere, Category = "Component")
	UWidgetComponent* MyBloodBar;
	UPROPERTY()
	UProgressBar* HPBarProgress;
	UPROPERTY(EditDefaultsOnly, Category = "Params")
	float fDamageRadius;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlayEffects();

	void PlayCollapseEffects();

	void Collapse();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	UFUNCTION(BlueprintCallable)
	bool GetInjured(AActor* DamageSource, float fDamageval);
};
