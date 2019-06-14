// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "TimerManager.h"
#include "Components/WidgetComponent.h"
#include "Components/Image.h"
#include "Components/SlateWrapperTypes.h"

AMyGameModeBase::AMyGameModeBase()
{
	TimeBetweenWaves = 10.0f;

}

void AMyGameModeBase::StartWave()
{
	NumberOfTinyToSpawn = 2 * WaveCount;
	GetWorldTimerManager().SetTimer(TimerHandle_TinySpawner, this, &AMyGameModeBase::SpawnTinyTimerElapsed, 1.0f, true, 0.0f);
}

void AMyGameModeBase::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TinySpawner);
	PrepareForNextWave();
}

void AMyGameModeBase::PrepareForNextWave()
{
	FTimerHandle TimerHandle_NextWaveStart;
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &AMyGameModeBase::StartWave, TimeBetweenWaves, false);
}

void AMyGameModeBase::StartPlay()
{
	Super::StartPlay();

	PrepareForNextWave();
}


void AMyGameModeBase::SpawnTinyTimerElapsed()
{
	SpawnNewTiny();
	NumberOfTinyToSpawn--;
	if (NumberOfTinyToSpawn <= 0)
	{
		EndWave();
	}
}

void AMyGameModeBase::JudgeGameOver(bool InMySide)
{
	if (!InMySide)
	{
		GameOverWin();
	}
	else
	{
		GameOverLose();
	}
}