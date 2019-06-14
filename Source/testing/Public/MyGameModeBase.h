// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

using namespace UF;
using namespace UM;
using namespace UP;


UCLASS()
class TESTING_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AMyGameModeBase();

protected:

	FTimerHandle TimerHandle_TinySpawner;

	int32 NumberOfTinyToSpawn;

	int32 WaveCount;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	int32 TimeBetweenWaves;


protected:


	// Spawn a single tinyhero;
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void SpawnNewTiny();

	
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void GameOverWin();
	

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void GameOverLose();

	void SpawnTinyTimerElapsed();

	void StartWave();
	
	void EndWave();
	 
	void PrepareForNextWave();

public:

	virtual void StartPlay() override;

	void JudgeGameOver(bool InMySide);


};
