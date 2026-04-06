


#include "Gameplay/Component/UWitherAbilitySystemComponent.h"
#include "AttributeSet.h"
#include "Data/AttributeSets/UGSAttributeSetBase.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"
#include "AbilitySystemLog.h"
#include "Data/Structs/FCharacterDataStructs.h"


// Sets default values for this component's properties
UWitherAbilitySystemComponent::UWitherAbilitySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TArray<TSubclassOf<UAttributeSet>> AttributeSetClasses;
	DefualtAttributeSets.GetKeys(AttributeSetClasses);
	for (TSubclassOf<UAttributeSet>& SetClass : AttributeSetClasses)
	{
		if (SetClass)
		{
			// Use the class name as the subobject name to ensure uniqueness
			FName SetName = SetClass->GetFName();
			UAttributeSet* NewSet = CreateDefaultSubobject<UAttributeSet>(SetName, true);

			// Add to our map for easy access later
			DefualtAttributeSets.Add(SetClass, NewSet);
		}
	}
}

// Called when the game starts
void UWitherAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<FGameplayTag> Subtags;
	UGameplayTagsManager& TagManager = UGameplayTagsManager::Get();
	FGameplayTagContainer ChildContainer = TagManager.RequestGameplayTagChildren(DefaultRootTag);
	ChildContainer.GetGameplayTagArray(SetByCallerDataTag);
	for (TSubclassOf<UAttributeSet> Set : DefualtAttributeSetsClass)
	{
		//FString Name = FString::Printf(TEXT("AttributeSet_%d"), Set.Get()->GetFName());
		UAttributeSet* CurSet = NewObject<UAttributeSet>(GetOwner(), Set);
		AddSpawnedAttribute(CurSet);
		DefualtAttributeSets.Add(Set, CurSet);
	}
	InitializeAttributes();
	AddStartupEffects();


	SantyChecks();
}

void UWitherAbilitySystemComponent::SantyChecks()
{
	ensureMsgf(!DefualtAttributeSets.IsEmpty(), TEXT("DefualtAttributeSets is empty on %s"), *GetName());
	ensureMsgf(DefaultAttributesEffect, TEXT("DefaultAttributesEffect is not set on %s"), *GetName());
	ensureMsgf(!SetByCallerDataTag.IsEmpty(), TEXT("SetByCallerDataTagname is empty on %s"), *GetName());

	checkf(DefaultAttributesDataTable != nullptr, TEXT("%s does not have reference to any Attributes Data Table"), *GetName());
	checkf(DefaultRowName != "", TEXT("%s failed to initialize Camera Boom!"), *GetName());
}

// Called every frame
void UWitherAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWitherAbilitySystemComponent::ResetHP()
{
	const UGSAttributeSetBase* AttrSet = GetSet<UGSAttributeSetBase>();
	if (AttrSet)
	{
		UGSAttributeSetBase* MutableAttrSet = const_cast<UGSAttributeSetBase*>(AttrSet);
		MutableAttrSet->ResetHealth();
	}

}

void UWitherAbilitySystemComponent::InitializeAttributes()
{
	
	if (!DefaultAttributesEffect)
	{
		UE_LOG(LogAbilitySystemComponent, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = MakeOutgoingSpec(DefaultAttributesEffect, GetCharacterLevel(), EffectContext);
	
	FCharacterStats* ItemData = DefaultAttributesDataTable->FindRow<FCharacterStats>(DefaultRowName, TEXT("Looking up item stats"));
	if (ItemData)
	{
		if (NewHandle.IsValid())
		{
			for (FGameplayTag Tag : SetByCallerDataTag)
			{
				float TageValue = ItemData->GetValueByTag(Tag);
				//UE_LOG(LogAbilitySystemComponent, Warning, TEXT("%f Tag value %s"), TageValue, *(Tag.ToString()));
				NewHandle.Data.Get()->SetSetByCallerMagnitude(Tag, TageValue);
			}
			FActiveGameplayEffectHandle ActiveGEHandle = ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
		}
	}
	else
	{
		UE_LOG(LogAbilitySystemComponent, Error, TEXT("%s's Data Table %s's could not find row with name <%s>"), *(GetOwner()->GetName()), *(DefaultAttributesDataTable->GetName()), *(DefaultRowName.ToString()));
	}
}

void UWitherAbilitySystemComponent::AddStartupEffects()
{
	if (bStartupEffectsApplied)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), this);
		}
	}

	bStartupEffectsApplied = true;
}

int32 UWitherAbilitySystemComponent::GetCharacterLevel() const
{
	//TODO
	return 1;
}

bool UWitherAbilitySystemComponent::IsAlive() const
{
	return GetNumericAttribute(UGSAttributeSetBase::GetHealthAttribute()) > 0.0f;
}


