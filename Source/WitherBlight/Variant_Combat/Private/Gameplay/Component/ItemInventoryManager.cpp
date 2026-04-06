


#include "Gameplay\Component\ItemInventoryManager.h"
#include "Gameplay\Items\ItemPickUp.h"
#include "CombatPlayerState.h"
#include "Gameplay\Component\UWitherAbilitySystemComponent.h"
#include "Data\Structs\FAttackAndItemDataStructs.h"

DEFINE_LOG_CATEGORY(LogItemInventory);

// Sets default values for this component's properties
UItemInventoryManager::UItemInventoryManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UItemInventoryManager::BeginPlay()
{
	Super::BeginPlay();
	OwnerPlayerState = Cast<ACombatPlayerState>(GetOwner());
	if (!OwnerPlayerState)
	{
		UE_LOG(LogItemInventory, Error, TEXT("ItemInventoryComponent %s's was not able to find parent playerstate"), *GetName());
	}
}

void UItemInventoryManager::AddItemToInventory(AItemPickUp* Item)
{
	if (!OwnerPlayerState)
	{
		UE_LOG(LogItemInventory, Error, TEXT("Item %s's was not past the Player State"), *GetName());
		return;
	}
	UAbilitySystemComponent* TempASC = Cast<IAbilitySystemInterface>(OwnerPlayerState)->GetAbilitySystemComponent();
	UWitherAbilitySystemComponent* ASC = Cast<UWitherAbilitySystemComponent>(TempASC);
	if (!ASC)
	{
		UE_LOG(LogItemInventory, Error, TEXT("Item %s's was not able to get the ASC from the Player State %s's"), *GetName(), *(OwnerPlayerState->GetName()));
	}
	FItemInfomation ItemInfo = Item->GetItemInfo();
	ApplyAllGameplayEffects(ASC, ItemInfo);
	ApplyAllGameplayAbilitys(ASC, ItemInfo);
	Item->OnPickup();
	AllItems.Add(ItemInfo);
}

void UItemInventoryManager::RemoveItemFromInventory(AItemPickUp* Item)
{

	if (!OwnerPlayerState)
	{
		UE_LOG(LogItemInventory, Error, TEXT("Item %s's was not past the Player State"), *GetName());
		return;
	}
	UAbilitySystemComponent* TempASC = Cast<IAbilitySystemInterface>(OwnerPlayerState)->GetAbilitySystemComponent();
	UWitherAbilitySystemComponent* ASC = Cast<UWitherAbilitySystemComponent>(TempASC);
	if (!ASC)
	{
		UE_LOG(LogItemInventory, Error, TEXT("Item %s's was not able to get the ASC from the Player State %s's"), *GetName(), *(OwnerPlayerState->GetName()));
	}
	FItemInfomation ItemInfo = Item->GetItemInfo();
	RemoveAllGameplayEffects(ASC, ItemInfo);
	RemoveAllGameplayAbilitys(ASC, ItemInfo);

	AllItems.Remove(ItemInfo);
}

void UItemInventoryManager::ApplyAllGameplayEffects(UWitherAbilitySystemComponent* ASC, FItemInfomation& ItemInfo)
{
	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	Context.AddSourceObject(this);
	for (TSubclassOf<UGameplayEffect> Effect : ItemInfo.ItemEffects)
	{
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(Effect, 1.0f, Context);
		if (SpecHandle.IsValid())
		{
			//SpecHandle.Data.Get()->Set SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), 50.0f);
			FActiveGameplayEffectHandle ActiveHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			ItemInfo.ItemActiveEffects.Add(ActiveHandle);
		}
	}
}

void UItemInventoryManager::ApplyAllGameplayAbilitys(UWitherAbilitySystemComponent* ASC, FItemInfomation& ItemInfo)
{
	if (OwnerPlayerState->GetLocalRole() != ROLE_Authority || !ASC) return;

	for (TSubclassOf<UGameplayAbility>& AbilityClass : ItemInfo.ItemAbilities)
	{
		FGameplayAbilitySpecHandle GrantedAbility = ASC->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1));
		ItemInfo.ItemActiveAbilities.Add(GrantedAbility);
	}
}

void UItemInventoryManager::RemoveAllGameplayEffects(UWitherAbilitySystemComponent* ASC, FItemInfomation& ItemInfo)
{
	for (FActiveGameplayEffectHandle ActiveEffect : ItemInfo.ItemActiveEffects)
	{
		ASC->RemoveActiveGameplayEffect(ActiveEffect, -1);
	}
	ItemInfo.ItemActiveEffects.Empty();
}

void UItemInventoryManager::RemoveAllGameplayAbilitys(UWitherAbilitySystemComponent* ASC, FItemInfomation& ItemInfo)
{
	for (FGameplayAbilitySpecHandle GrantedAbility : ItemInfo.ItemActiveAbilities)
	{
		ASC->ClearAbility(GrantedAbility);
	}
}



