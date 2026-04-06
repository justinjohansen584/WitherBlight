


#include "CombatPlayerState.h"
#include "Gameplay\Component\UWitherAbilitySystemComponent.h"
#include "Data\AttributeSets\UGSAttributeSetBase.h"
#include "Gameplay\Component\ItemInventoryManager.h"


// Sets default values
ACombatPlayerState::ACombatPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UWitherAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	InventoryManager = CreateDefaultSubobject<UItemInventoryManager>(TEXT("ItemInventoryManager"));
}

// Called when the game starts or when spawned
void ACombatPlayerState::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AttributeSetBase = const_cast<UGSAttributeSetBase*>(AbilitySystemComponent->GetSet<UGSAttributeSetBase>());

	#if WITH_EDITOR
	SantyChecks();
	#endif
}

#if WITH_EDITOR
void ACombatPlayerState::SantyChecks()
{
	checkf(AbilitySystemComponent != nullptr, TEXT("%s failed to initialize ASC!"), *GetName());
	checkf(AttributeSetBase != nullptr, TEXT("%s failed to grab AttributeSet!"), *GetName());
	checkf(InventoryManager != nullptr, TEXT("%s failed to initialize Item Inventory Manager!"), *GetName());
}
#endif











UAbilitySystemComponent* ACombatPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UGSAttributeSetBase* ACombatPlayerState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

UItemInventoryManager* ACombatPlayerState::GetItemInventoryManager() const
{
	return InventoryManager;
}

float ACombatPlayerState::GetHealth() const
{
	return AttributeSetBase->GetHealth();
}

float ACombatPlayerState::GetMaxHealth() const
{
	return AttributeSetBase->GetMaxHealth();
}

float ACombatPlayerState::GetHealthRegenRate() const
{
	return AttributeSetBase->GetHealthRegenRate();
}

float ACombatPlayerState::GetMana() const
{
	return AttributeSetBase->GetMana();
}

float ACombatPlayerState::GetMaxMana() const
{
	return AttributeSetBase->GetMaxMana();
}

float ACombatPlayerState::GetManaRegenRate() const
{
	return AttributeSetBase->GetManaRegenRate();
}

float ACombatPlayerState::GetStamina() const
{
	return AttributeSetBase->GetStamina();
}

float ACombatPlayerState::GetMaxStamina() const
{
	return AttributeSetBase->GetMaxStamina();
}

float ACombatPlayerState::GetStaminaRegenRate() const
{
	return AttributeSetBase->GetStaminaRegenRate();
}

float ACombatPlayerState::GetShield() const
{
	return AttributeSetBase->GetShield();
}

float ACombatPlayerState::GetMaxShield() const
{
	return AttributeSetBase->GetMaxShield();
}

float ACombatPlayerState::GetShieldRegenRate() const
{
	return AttributeSetBase->GetShieldRegenRate();
}

float ACombatPlayerState::GetArmor() const
{
	return AttributeSetBase->GetArmor();
}

float ACombatPlayerState::GetMoveSpeed() const
{
	return AttributeSetBase->GetMoveSpeed();
}

int32 ACombatPlayerState::GetCharacterLevel() const
{
	return AttributeSetBase->GetCharacterLevel();
}

int32 ACombatPlayerState::GetXP() const
{
	return AttributeSetBase->GetXP();
}

int32 ACombatPlayerState::GetXPBounty() const
{
	return AttributeSetBase->GetXPBounty();
}

int32 ACombatPlayerState::GetGold() const
{
	return AttributeSetBase->GetGold();
}

int32 ACombatPlayerState::GetGoldBounty() const
{
	return AttributeSetBase->GetGoldBounty();
}


