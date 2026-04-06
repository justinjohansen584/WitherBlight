// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CombatDamageable.h"
#include "AbilitySystemInterface.h"
#include "CombatDummy.generated.h"

class UStaticMeshComponent;
class UPhysicsConstraintComponent;
class UAbilitySystemComponent;
class UWitherAbilitySystemComponent;

/**
 *  A simple invincible combat training dummy
 */
UCLASS(abstract)
class ACombatDummy : public AActor, public ICombatDamageable, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
	/** Root component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;

	/** Static base plate */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BasePlate;

	/** Physics enabled dummy mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Dummy;

	/** Physics constraint holding the dummy and base plate together */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UPhysicsConstraintComponent* PhysicsConstraint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWitherAbilitySystemComponent> AbilitySystemComponent;

public:	
	
	/** Constructor */
	ACombatDummy();

	/** Initialization */
	virtual void BeginPlay() override;

	// ~Begin CombatDamageable interface

	//Delegate that is called when damage is called 
	virtual void HandleDamageTaken(float DamageAmount, const FGameplayTagContainer& SourceTags) override;

	/** Handles death events */
	virtual void HandleDeath() override;

	/** Allows reaction to incoming attacks */
	virtual void NotifyDanger(const FVector& DangerLocation, AActor* DangerSource) override;

	/*Does the owner have more than zero health*/
	virtual bool IsAlive() const override;

	// ~End CombatDamageable interface

	// ~begin IAbilitySystemInterface interface

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// -end IAbilitySystemInterface interface


protected:

	/** Blueprint handle to apply damage effects */
	UFUNCTION(BlueprintImplementableEvent, Category="Combat", meta = (DisplayName = "On Dummy Damaged"))
	void BP_OnDummyDamaged(const FVector& Location, const FVector& Direction);
};
