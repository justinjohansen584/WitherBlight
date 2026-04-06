// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemComponent.h"
#include "CombatDamageable.generated.h"


/**
 *  CombatDamageable interface
 *  Provides functionality to handle damage, healing, knockback and death
 *  Also provides functionality to warn characters of incoming sources of damage
 */
UINTERFACE(MinimalAPI, NotBlueprintable)
class UCombatDamageable : public UInterface
{
	GENERATED_BODY()
};

class ICombatDamageable
{
	GENERATED_BODY()

public:

	/** Handles Damage done events */
	UFUNCTION(BlueprintCallable, Category = "Damageable")
	virtual void HandleDamageTaken(float DamageAmount, const FGameplayTagContainer& SourceTags) = 0;

	/** Handles death events */
	UFUNCTION(BlueprintCallable, Category="Damageable")
	virtual void HandleDeath() = 0;

	/** Handles healing events */
	//UFUNCTION(BlueprintCallable, Category="Damageable")
	//virtual void ApplyHealing(float Healing, AActor* Healer) = 0;

	/** Notifies the actor of impending danger such as an incoming hit, allowing it to react. */
	UFUNCTION(BlueprintCallable, Category="Damageable")
	virtual void NotifyDanger(const FVector& DangerLocation, AActor* DangerSource) = 0;

	/** Checks to see if its alive */
	UFUNCTION(BlueprintCallable, Category = "Damageable")
	virtual bool IsAlive() const = 0;
};
