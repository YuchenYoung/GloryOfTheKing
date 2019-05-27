// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerActor.generated.h"

class USphereComponent;

UCLASS()
class TESTING_API ATowerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATowerActor();

protected:

	UPROPERTY(VisibleAnywhere, category = "Component")
	UStaticMeshComponent* Meshcomp;
	UPROPERTY(VisibleAnywhere, category = "Component")
	USphereComponent* Spherecomp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
