

#pragma once

#include "CoreMinimal.h"
#include "Data\Enums\EItemEnums.h"
#include "GameplayEffectTypes.h"
#include "ActiveGameplayEffectHandle.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayEffect.h"
#include "GameplayAbilitySpec.h"
#include "FAttackAndItemDataStructs.generated.h"

//class UBlockGatherShootComponent;
//class UGameplayEffect;
//class UAbilitySystemComponent;
//struct FGameplayEffectContextHandle;
//struct FActiveGameplayEffectHandle;
//struct FGameplayAbilitySpecHandle;

/**
 *A struct that contain all the info for a block
 */
USTRUCT(BlueprintType)
struct FProjectileInfomation
{
	GENERATED_BODY()
	//Propetys set in Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	FName name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float speed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	TSubclassOf<UGameplayEffect> InitDamageEffectClass;
	//Propertys set in native
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile")
	TObjectPtr<AActor> Owner;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile")
	TObjectPtr<class UBlockGatherShootComponent> OwnerGatherShootComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile")
	TObjectPtr<class UAbilitySystemComponent> OwnerASC;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile")
	FTransform SpawnTransform;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	struct FGameplayEffectContextHandle ContextHandle;

	FProjectileInfomation()
	{
		name = "";
		speed = 3000;
		InitDamageEffectClass = nullptr;
		Owner = nullptr;
		OwnerGatherShootComp = nullptr;
		OwnerASC = nullptr;
		SpawnTransform = FTransform();
		ContextHandle = nullptr;
	}

};

/**
 *A struct that contain all the info for a block
 */
USTRUCT(BlueprintType)
struct FItemInfomation
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FName name = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TObjectPtr<AActor> Owner = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TArray<EItemPickupType> ItemSlotTypes = {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TArray<TSubclassOf<UGameplayAbility>> ItemAbilities = {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TArray<TSubclassOf<UGameplayEffect>> ItemEffects = {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TArray<struct FGameplayAbilitySpecHandle> ItemActiveAbilities = {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TArray<struct FActiveGameplayEffectHandle> ItemActiveEffects = {};

	
	bool operator==(const FItemInfomation& Other) const
	{
		// Compare basic types first (fastest)
		if (name != Other.name || Owner != Other.Owner)
		{
			return false;
		}

		// Compare static data arrays (GAS classes)
		if (ItemAbilities != Other.ItemAbilities || ItemEffects != Other.ItemEffects || ItemSlotTypes != Other.ItemSlotTypes)
		{
			return false;
		}

		// If you REALLY need to compare active handles:
		return ItemActiveAbilities.Num() == Other.ItemActiveAbilities.Num() &&
		   ItemActiveEffects.Num() == Other.ItemActiveEffects.Num();
	}
	
};
