// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/CombatAttacker.h"
#include "Interfaces/CombatDamageable.h"
#include "Animation/AnimMontage.h"
#include "Engine/TimerHandle.h"
#include "Data/Structs/FCharacterDataStructs.h"
#include "AbilitySystemInterface.h"
#include "CombatEnemy.generated.h"

class UWidgetComponent;
class UCombatLifeBar;
class UAnimMontage;
class UAbilitySystemComponent;
class UWitherAbilitySystemComponent;
struct FGameplayTag;
struct FOnAttributeChangeData;

/** Completed attack animation delegate for StateTree */
DECLARE_DELEGATE(FOnEnemyAttackCompleted);

/** Landed delegate for StateTree */
DECLARE_DELEGATE(FOnEnemyLanded);

/** Enemy died delegate */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyDied);

/**
 *  An AI-controlled character with combat capabilities.
 *  Its bundled AI Controller runs logic through StateTree
 */
UCLASS(abstract)
class ACombatEnemy : public ACharacter, public ICombatAttacker, public ICombatDamageable, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Life bar widget component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* LifeBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWitherAbilitySystemComponent> AbilitySystemComponent;

public:
	
	/** Constructor */
	ACombatEnemy();

protected:

	/** Max amount of HP the character will have on respawn */
	UPROPERTY(EditAnywhere, Category="Damage")
	float MaxHP = 3.0f;

	//TagOnDead Creatures
	FGameplayTag DeadTag;

	FGameplayTag EffectRemoveOnDeathTag;

protected:

	/** Name of the pelvis bone, for damage ragdoll physics */
	UPROPERTY(EditAnywhere, Category="Damage")
	FName PelvisBoneName;

	/** Pointer to the life bar widget */
	UPROPERTY(EditAnywhere, Category="Damage")
	UCombatLifeBar* LifeBarWidget;

	/** If true, the character is currently playing an attack animation */
	bool bIsAttacking = false;

	/** Distance ahead of the character that melee attack sphere collision traces will extend */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Trace", meta = (ClampMin = 0, ClampMax = 500, Units = "cm"))
	float MeleeTraceDistance = 75.0f;

	/** Radius of the sphere trace for melee attacks */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Trace", meta = (ClampMin = 0, ClampMax = 500, Units = "cm"))
	float MeleeTraceRadius = 50.0f;

	/** Amount of damage a melee attack will deal */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Damage", meta = (ClampMin = 0, ClampMax = 100))
	float MeleeDamage = 1.0f;

	/** Amount of knockback impulse a melee attack will apply */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Damage", meta = (ClampMin = 0, ClampMax = 1000, Units = "cm/s"))
	float MeleeKnockbackImpulse = 150.0f;

	/** Amount of upwards impulse a melee attack will apply */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Damage", meta = (ClampMin = 0, ClampMax = 1000, Units = "cm/s"))
	float MeleeLaunchImpulse = 350.0f;

	/** AnimMontage that will play for Light attacks */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Light")
	UAnimMontage* LightAttackMontage;

	/** Names of the AnimMontage sections that correspond to each stage of the Light attack */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Light")
	TArray<FName> LightSectionNames;

	/** Target number of attacks in the Light attack string we're playing */
	int32 TargetLightCount = 0;

	/** Index of the current stage of the melee attack Light */
	int32 CurrentLightAttack = 0;

	/** AnimMontage that will play for charged attacks */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Charged")
	UAnimMontage* ChargedAttackMontage;

	/** Name of the AnimMontage section that corresponds to the charge loop */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Charged")
	FName ChargeLoopSection;

	/** Name of the AnimMontage section that corresponds to the attack */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Charged")
	FName ChargeAttackSection;

	/** Minimum number of charge animation loops that will be played by the AI */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Charged", meta = (ClampMin = 1, ClampMax = 20))
	int32 MinChargeLoops = 2;

	/** Maximum number of charge animation loops that will be played by the AI */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Charged", meta = (ClampMin = 1, ClampMax = 20))
	int32 MaxChargeLoops = 5;

	/** Target number of charge animation loops to play in this charged attack */
	int32 TargetChargeLoops = 0;

	/** Number of charge animation loop currently playing */
	int32 CurrentChargeLoop = 0;

	/** Time to wait before removing this character from the level after it dies */
	UPROPERTY(EditAnywhere, Category="Death")
	float DeathRemovalTime = 5.0f;

	/** Enemy death timer */
	FTimerHandle DeathTimer;

	/** Attack montage ended delegate */
	FOnMontageEnded OnAttackMontageEnded;

	/** Last recorded location we're being attacked from */
	FVector LastDangerLocation = FVector::ZeroVector;

	/** Last recorded game time we were attacked */
	float LastDangerTime = -1000.0f;

	UPROPERTY(EditAnywhere, Category = "Stats")
	FCharacterStats CharacterStats;

public:
	/** Attack completed internal delegate to notify StateTree tasks */
	FOnEnemyAttackCompleted OnAttackCompleted;

	/** Landed internal delegate to notify StateTree tasks. We use this instead of the built-in Landed delegate so we can bind to a Lambda in StateTree tasks */
	FOnEnemyLanded OnEnemyLanded;

	/** Enemy died delegate. Allows external subscribers to respond to enemy death */
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnEnemyDied OnEnemyDied;

public:

	/** Performs an AI-initiated Light attack. Number of hits will be decided by this character */
	void DoAILightAttack();

	/** Performs an AI-initiated charged attack. Charge time will be decided by this character */
	void DoAIChargedAttack();

	/** Called from a delegate when the attack montage ends */
	void AttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	/** Returns the last recorded location we were attacked from */
	const FVector& GetLastDangerLocation() const;

	/** Returns the last game time we were attacked */
	float GetLastDangerTime() const;

public:

	// ~begin ICombatAttacker interface

	/** Performs an attack's collision check */
	virtual void DoAttackTrace(FName DamageSourceBone) override;

	/** Performs a Light attack's check to continue the string */
	UFUNCTION(BlueprintCallable, Category="Attacker")
	virtual void CheckLight() override;

	/** Performs a charged attack's check to loop the charge animation */
	UFUNCTION(BlueprintCallable, Category="Attacker")
	virtual void CheckChargedAttack() override;

	virtual FCharacterStats GetCharacterStats() override;

	virtual void BlockGathered(int32 newTotal) override;

	// ~end ICombatAttacker interface

	// ~begin ICombatDamageable interface

	//Delegate that is called when damage is called 
	virtual void HandleDamageTaken(float DamageAmount, const FGameplayTagContainer& SourceTags) override;

	/** Handles death events */
	virtual void HandleDeath() override;

	/** Allows the enemy to react to incoming attacks */
	virtual void NotifyDanger(const FVector& DangerLocation, AActor* DangerSource) override;

	/*Does the owner have more than zero health*/
	virtual bool IsAlive() const override;

	// ~end ICombatDamageable interface

	// ~begin IAbilitySystemInterface interface

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// -end IAbilitySystemInterface interface

protected:

	/** Removes this character from the level after it dies */
	void RemoveFromLevel();

public:

	/** Overrides landing to reset damage ragdoll physics */
	virtual void Landed(const FHitResult& Hit) override;

protected:

	/** Blueprint handler to play damage received effects */
	UFUNCTION(BlueprintImplementableEvent, Category="Combat")
	void ReceivedDamage(float Damage, const FVector& ImpactPoint, const FVector& DamageDirection);

protected:

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** EndPlay cleanup */
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
};
