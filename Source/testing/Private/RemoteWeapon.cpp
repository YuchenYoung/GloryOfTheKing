// Fill out your copyright notice in the Description page of Project Settings.


#include "RemoteWeapon.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ARemoteWeapon::ARemoteWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Meshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Meshcomp"));
	RootComponent = Cast<USceneComponent>(Meshcomp);
	Boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("Boxcomp"));
	Boxcomp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Boxcomp->SetCollisionResponseToAllChannels(ECR_Ignore);
	Boxcomp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Boxcomp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARemoteWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARemoteWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

