

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//#include "Data\Structs\FAttackAndItemDataStructs.h"
#include "ItemInventoryManager.generated.h"

struct FItemInfomation;
class AItemPickUp;

DECLARE_LOG_CATEGORY_EXTERN(LogItemInventory, Log, All);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WITHERBLIGHT_API UItemInventoryManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemInventoryManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual void AddItemToInventory(AItemPickUp* Item);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual void RemoveItemFromInventory(AItemPickUp* Item);

private:
	/*Apply all effects to target ASC*/
	virtual void ApplyAllGameplayEffects(class UWitherAbilitySystemComponent* ASC, FItemInfomation& ItemInfo);

	/*Apply all Abilitys to target ASC*/
	virtual void ApplyAllGameplayAbilitys(UWitherAbilitySystemComponent* ASC, FItemInfomation& ItemInfo);

	/*Remove all effects to target ASC*/
	virtual void RemoveAllGameplayEffects(UWitherAbilitySystemComponent* ASC, FItemInfomation& ItemInfo);

	/*Remove all Abilitys to target ASC*/
	virtual void RemoveAllGameplayAbilitys(UWitherAbilitySystemComponent* ASC, FItemInfomation& ItemInfo);

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TArray<struct FItemInfomation> AllItems;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<class ACombatPlayerState> OwnerPlayerState;
};
