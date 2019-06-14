// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyHealthComponent.h"
#include "testingCharacter.generated.h"


class UPawnNoiseEmitterComponent;
class UMyhealthComponent;
using namespace UM;
using namespace UF;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

public:
	UPROPERTY( BlueprintReadWrite, Category = "HeroHealth")
	class UMyHealthComponent* HeroHealth;

	UPROPERTY(EditInstanceOnly, Category = "Side")
	bool bInMySide;





public:
	
	UFUNCTION(BlueprintCallable)
	void OnHealthChanged(UMyHealthComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
	bool GetInjured(AActor* DamageSource, float fDamageval);

	UFUNCTION()
	void OnLevelChanged();
	UFUNCTION()
	void AddResult_Tiny();
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
	UPROPERTY(BlueprintReadWrite, Category = "Result")
	int Result_Tiny;
	UPROPERTY(BlueprintReadWrite, Category = "Result")
	int Result_Hero;
	UPROPERTY(BlueprintReadWrite, Category = "Result")
	int Result_Tower;
protected:
	UPROPERTY(BlueprintReadOnly,Category="Player")
	bool bDied;

	void Die();
	void PlayDeathEffects();
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPlayDeathEffects();

	void BeginPlay();
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

	void PlayEffects1();
	void PlayEffects2();
	void PlayEffects3();
	void PlayEffects4();

	UFUNCTION(Server,Reliable,WithValidation)
	void ServerPlayEffects1();
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPlayEffects2();
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPlayEffects3();
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPlayEffects4();


	void MoveForward(float val);
	void MoveRight(float val);

public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;


};

