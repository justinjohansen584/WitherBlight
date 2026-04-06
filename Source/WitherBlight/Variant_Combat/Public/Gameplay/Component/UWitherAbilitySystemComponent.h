

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "UWitherAbilitySystemComponent.generated.h"

class UGameplayEffect;
class UAttributeSet;
struct FGameplayTag;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UWitherAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWitherAbilitySystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/** Resets the character's current HP to maximum */
	virtual void ResetHP();

	virtual bool IsAlive() const;

	//bool bCharacterAbilitiesGiven = false;
	bool bStartupEffectsApplied = false;

private:
	//Functions that contains all the 
	virtual void SantyChecks();

protected:
	// Initialize the Character's attributes.
	virtual void InitializeAttributes();

	virtual void AddStartupEffects();

	virtual int32 GetCharacterLevel() const;

protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Setup")
	TArray<TSubclassOf<UAttributeSet>> DefualtAttributeSetsClass;
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Setup")
	TMap<TSubclassOf<UAttributeSet>, TObjectPtr<UAttributeSet>> DefualtAttributeSets;
	// Default attributes for a character for initializing on spawn/respawn.
    // This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Setup")
	TSubclassOf<UGameplayEffect> DefaultAttributesEffect;   //SetbyTag
	//Owners of the Ability Systems Attribute stats
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Setup")
	TObjectPtr<UDataTable> DefaultAttributesDataTable;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Setup")
	FName DefaultRowName;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Setup")
	FGameplayTag DefaultRootTag;
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Setup")
	TArray<FGameplayTag> SetByCallerDataTag;
	// These effects are only applied one time on startup
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Setup")
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;

	
};
