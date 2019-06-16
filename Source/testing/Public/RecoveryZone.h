// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RecoveryZone.generated.h"

class UBoxComponent;
class UDecalComponent;
using namespace UP;

UCLASS()
class TESTING_API ARecoveryZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARecoveryZone();

protected:


	UPROPERTY(VisibleAnywhere,Category = "Components")
	UBoxComponent* OverlapComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UDecalComponent* DecalComp;
	

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent ,AActor* OtherActor ,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);



};
