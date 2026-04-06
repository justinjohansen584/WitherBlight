// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "FCharacterDataStructs.generated.h"


/**
 * Struct Containing all the infomation for a certatin unit.
 */
USTRUCT(BlueprintType)
struct FCharacterStats: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float fChanceOfBlockGrab = .95f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float BlockTraceDistance = 75.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float BlockTraceRadius = 75.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float DangerTraceDistance = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float DangerTraceRadius = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float BaseDamage = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float BaseKnockbackImpulse = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes Data")
	float Health = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes Data")
	float MaxHealth = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes Data")
	float HealthRegenRate = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes Data")
	float Mana = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes Data")
	float MaxMana = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes Data")
	float ManaRegenRate = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes Data")
	float Stamina = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes Data")
	float MaxStamina = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes Data")
	float StaminaRegenRate = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes Data")
	float Shield = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes Data")
	float MaxShield = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes Data")
	float ShieldRegenRate = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes Data")
	float Armor = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes Data")
	float MoveSpeed = 300;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes Data")
	float CharacterLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes Data")
	float XP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes Data")
	float XPBounty = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes Data")
	float Gold = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes Data")
	float GoldBounty = 1;

	// The function to find a value by Tag
	virtual float GetValueByTag(const FGameplayTag& Tag) const;
};