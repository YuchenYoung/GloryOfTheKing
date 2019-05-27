// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerActor.h"
#include "Components/SphereComponent.h"

// Sets default values
ATowerActor::ATowerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Meshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Meshcomp"));
	RootComponent = Meshcomp;
	Spherecomp = CreateDefaultSubobject<USphereComponent>(TEXT("Spherecomp"));
	Spherecomp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATowerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATowerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

