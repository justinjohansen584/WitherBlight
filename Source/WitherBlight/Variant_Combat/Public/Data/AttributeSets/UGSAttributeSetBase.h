

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "UGSAttributeSetBase.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName);

DECLARE_LOG_CATEGORY_EXTERN(LogAttributeSet, Log, All);

/**
 *
 */
UCLASS()
class UGSAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()
public:
	UGSAttributeSetBase();

	// Current Health, when 0 we expect owner to die unless prevented by an ability. Capped by MaxHealth.
	// Positive changes can directly use this.
	// Negative changes to Health should go through Damage meta attribute.
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, Health)

	// MaxHealth is its own attribute since GameplayEffects may modify it
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, MaxHealth)

	// Health regen rate will passively increase Health every second
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData HealthRegenRate;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, HealthRegenRate)

	// Current Mana, used to execute special abilities. Capped by MaxMana.
	UPROPERTY(BlueprintReadOnly, Category = "Mana")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, Mana)

	// MaxMana is its own attribute since GameplayEffects may modify it
	UPROPERTY(BlueprintReadOnly, Category = "Mana")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, MaxMana)

	// Mana regen rate will passively increase Mana every second
	UPROPERTY(BlueprintReadOnly, Category = "Mana")
	FGameplayAttributeData ManaRegenRate;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, ManaRegenRate)

	// Current stamina, used to execute special abilities. Capped by MaxStamina.
	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, Stamina)

	// MaxStamina is its own attribute since GameplayEffects may modify it
	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, MaxStamina)

	// Stamina regen rate will passively increase Stamina every second
	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	FGameplayAttributeData StaminaRegenRate;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, StaminaRegenRate)

	// Current shield acts like temporary health. When depleted, damage will drain regular health.
	UPROPERTY(BlueprintReadOnly, Category = "Shield")
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, Shield)

	// Maximum shield that we can have.
	UPROPERTY(BlueprintReadOnly, Category = "Shield")
	FGameplayAttributeData MaxShield;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, MaxShield)

	// Shield regen rate will passively increase Shield every second
	UPROPERTY(BlueprintReadOnly, Category = "Shield")
	FGameplayAttributeData ShieldRegenRate;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, ShieldRegenRate)

	// Armor reduces the amount of damage done by attackers
	UPROPERTY(BlueprintReadOnly, Category = "Armor")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, Armor)

	// Damage is a meta attribute used by the DamageExecution to calculate final damage, which then turns into -Health
	// Temporary value that only exists on the Server. Not replicated.
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, Damage)

	// MoveSpeed affects how fast characters can move.
	UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, MoveSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Character Level")
	FGameplayAttributeData CharacterLevel;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, CharacterLevel)

	// Experience points gained from killing enemies. Used to level up (not implemented in this project).
	UPROPERTY(BlueprintReadOnly, Category = "XP")
	FGameplayAttributeData XP;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, XP)

	// Experience points awarded to the character's killers. Used to level up (not implemented in this project).
	UPROPERTY(BlueprintReadOnly, Category = "XP")
	FGameplayAttributeData XPBounty;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, XPBounty)

	// Gold gained from killing enemies. Used to purchase items (not implemented in this project).
	UPROPERTY(BlueprintReadOnly, Category = "Gold")
	FGameplayAttributeData Gold;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, Gold)

	// Gold awarded to the character's killer. Used to purchase items (not implemented in this project).
	UPROPERTY(BlueprintReadOnly, Category = "Gold")
	FGameplayAttributeData GoldBounty;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, GoldBounty)

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Reset health
	virtual void ResetHealth();

protected:
	// Helper function to proportionally adjust the value of an attribute when it's associated max attribute changes.
	// (i.e. When MaxHealth increases, Health increases by an amount that maintains the same percentage as before)
	
	virtual void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);

};
