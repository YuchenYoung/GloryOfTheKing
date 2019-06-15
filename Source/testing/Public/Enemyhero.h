// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyHealthComponent.h"

#include "Enemyhero.generated.h"


class UPawnNoiseEmitterComponent;
class UMyhealthComponent;
using namespace UM;
class UPawnSensingComponent;


UCLASS(config = Game)
class AEnemyhero : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyhero();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* CursorToWorld;
	int32 Skill1Time;
	int32 Skill2Time;
	int32 Skill3Time;
	int32 RestartTime;


public:
	UPROPERTY(BlueprintReadWrite, Category = "HeroHealth")
		class UMyHealthComponent* HeroHealth;





public:

	UFUNCTION()
		void OnHealthChanged(UMyHealthComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
		void GetInjured(AActor* DamageSource, float fDamageval);

	UFUNCTION()
		void OnLevelChanged();
	UPROPERTY(BlueprintReadWrite, Category = "Money")
		int Money;
	UPROPERTY(BlueprintReadWrite, Category = "Attribute")
		float AttackValue;
	UPROPERTY(BlueprintReadWrite, Category = "Attribute")
		float Defense;
	UPROPERTY(BlueprintReadWrite, Category = "Attribute")
		float Energy;
	UPROPERTY(BlueprintReadWrite, Category = "Attribute")
		float aEnergy;
	UPROPERTY(BlueprintReadWrite, Category = "Attribute")
		int Level;
	float dLevel;
	UPROPERTY(BlueprintReadWrite, Category = "Attribute")
		float aLevel;
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Player")
		bool bDied;
	void Die();
	void PlayDeathEffects();
	void BeginPlay();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
		UPawnNoiseEmitterComponent * NoiseEmitterComponent;
	UPROPERTY(VisibleAnywhere, Category = "Component")
		UCapsuleComponent* AttackCapsulecomp;
	UFUNCTION()
		void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		UParticleSystem* SkillEffectQ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		UParticleSystem* SkillEffectW;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		UParticleSystem* SkillEffectE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		UParticleSystem* SkillEffectR;
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UPawnSensingComponent* PawnSensingComp;




	void PlayEffects1();
	void PlayEffects2();
	void PlayEffects3();
	void PlayEffects4();

public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void MoveForward(float val);
	void MoveRight(float val);
	int direction;
	/* forward 1  backward 2  left 3  right 4  */
};

