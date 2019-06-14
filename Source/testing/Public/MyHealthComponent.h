#pragma once

#include "GameFramework/DamageType.h"
#include "GameFramework/Controller.h"
#include "Components/ActorComponent.h"
#include "MyHealthComponent.generated.h"

using namespace UP;
//OnHealtChanged Event
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, UMyHealthComponent*, HealthComp, float, Health, float, HealthDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);
UCLASS( ClassGroup=(COOP), meta=(BlueprintSpawnableComponent) )
class TESTING_API UMyHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyHealthComponent();
	//void damage(int num);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HealthComponent")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent")
	float DefaultHealth;

	void InitialHealth();

	UFUNCTION()
	void Damage(float Damageval, float Defense);

	bool JudgeDeath();

	float HealthLeft();

	/*
	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	*/
	/*
	UPROPERTY(BlueprintAssignable,Category="Events")
	FOnHealthChangedSignature OnHealthChanged;
	*/
	/*const class UDamageType* DefaultDamageType;
	class AController* DefaultConTroller;*/

};
