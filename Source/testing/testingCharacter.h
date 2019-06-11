// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyHealthComponent.h"

#include "testingCharacter.generated.h"


class UPawnNoiseEmitterComponent;
using namespace UM;


UCLASS(config = Game)
class AtestingCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AtestingCharacter();

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

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HeroHealth")
	class UMyHealthComponent* HeroHealth;




public:
	void GetInjured(AActor* DamageSource, float fDamageval);
	
	


protected:
	void Die();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="AI", meta = (AllowPrivateAccess = "true"))
	UPawnNoiseEmitterComponent* NoiseEmitterComponent;
	UPROPERTY(VisibleAnywhere, Category = "Component")
		UCapsuleComponent* AttackCapsulecomp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* SkillEffectQ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* SkillEffectW;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* SkillEffectE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* SkillEffectR;

	void PlayEffectsQ();
	void PlayEffectsW();
	void PlayEffectsE();
	void PlayEffectsR();

public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;


};

