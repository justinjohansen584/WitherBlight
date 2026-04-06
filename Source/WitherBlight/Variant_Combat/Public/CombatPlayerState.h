

#pragma once

#include "CoreMinimal.h"
#include "GameFramework\PlayerState.h"
#include "AbilitySystemInterface.h"
#include "CombatPlayerState.generated.h"

UCLASS()
class WITHERBLIGHT_API ACombatPlayerState : public APlayerState, public IAbilitySystemInterface
{	
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr< class UWitherAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UGSAttributeSetBase> AttributeSetBase;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UItemInventoryManager> InventoryManager;
	
public:	
	// Sets default values for this actor's properties
	ACombatPlayerState();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/* Start of IAbilitySystemInterface*/
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/* End of IAbilitySystemInterface*/
	UGSAttributeSetBase* GetAttributeSetBase() const;
	UItemInventoryManager* GetItemInventoryManager() const;

private:
	virtual void SantyChecks();

public:
	/**
	* Getters for attributes from GDAttributeSetBase. Returns Current Value unless otherwise specified.
	*/

	UFUNCTION(BlueprintCallable, Category = "Combat|PlayerState|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Combat|PlayerState|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Combat|PlayerState|Attributes")
	float GetHealthRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "Combat|PlayerState|Attributes")
	float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "Combat|PlayerState|Attributes")
	float GetMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "Combat|PlayerState|Attributes")
	float GetManaRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "Combat|PlayerState|Attributes")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Combat|PlayerState|Attributes")
	float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Combat|PlayerState|Attributes")
	float GetStaminaRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "Combat|PlayerState|Attributes")
	float GetShield() const;

	UFUNCTION(BlueprintCallable, Category = "Combat|PlayerState|Attributes")
	float GetMaxShield() const;

	UFUNCTION(BlueprintCallable, Category = "Combat|PlayerState|Attributes")
	float GetShieldRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "Combat|PlayerState|Attributes")
	float GetArmor() const;

	UFUNCTION(BlueprintCallable, Category = "Combat|PlayerState|Attributes")
	float GetMoveSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Combat|PlayerState|Attributes")
	int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "Combat|PlayerState|Attributes")
	int32 GetXP() const;

	UFUNCTION(BlueprintCallable, Category = "Combat|PlayerState|Attributes")
	int32 GetXPBounty() const;

	UFUNCTION(BlueprintCallable, Category = "Combat|PlayerState|Attributes")
	int32 GetGold() const;

	UFUNCTION(BlueprintCallable, Category = "Combat|PlayerState|Attributes")
	int32 GetGoldBounty() const;
	
};
