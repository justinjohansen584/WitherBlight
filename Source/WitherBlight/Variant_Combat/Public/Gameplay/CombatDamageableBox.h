// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CombatDamageable.h"
#include "AbilitySystemInterface.h"
#include "CombatDamageableBox.generated.h"

class UAbilitySystemComponent;
class UWitherAbilitySystemComponent;
struct FOnAttributeChangeData;

/**
 *  A simple physics box that reacts to damage through the ICombatDamageable interface
 */
UCLASS(abstract)
class ACombatDamageableBox : public AActor, public ICombatDamageable, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
	/** Damageable box mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWitherAbilitySystemComponent> AbilitySystemComponent;

public:	

	/** Constructor */
	ACombatDamageableBox();

	/** Initialization */
	virtual void BeginPlay() override;

protected:

	/** Amount of HP this box starts with. */
	UPROPERTY(EditAnywhere, Category="Damage")
	float CurrentHP = 3.0f;

	/** Time to wait before we remove this box from the level. */
	UPROPERTY(EditAnywhere, Category="Damage", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float DeathDelayTime = 6.0f;

	/** Timer to defer destruction of this box after its HP are depleted */
	FTimerHandle DeathTimer;

	/** Blueprint damage handler for effect playback */
	UFUNCTION(BlueprintImplementableEvent, Category="Damage")
	void OnBoxDamaged(const FVector& DamageLocation, const FVector& DamageImpulse);

	/** Blueprint destruction handler for effect playback */
	UFUNCTION(BlueprintImplementableEvent, Category="Damage")
	void OnBoxDestroyed();

	/** Timer callback to remove the box from the level after it dies */
	void RemoveFromLevel();

public:

	/** EndPlay cleanup */
	void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	// ~Begin CombatDamageable interface

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
};
