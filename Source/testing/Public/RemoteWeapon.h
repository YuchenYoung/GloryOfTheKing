// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RemoteWeapon.generated.h"

class UBoxComponent;

UCLASS()
class TESTING_API ARemoteWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARemoteWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* Meshcomp;
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UBoxComponent* Boxcomp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
