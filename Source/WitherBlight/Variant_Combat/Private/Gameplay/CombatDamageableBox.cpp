// Copyright Epic Games, Inc. All Rights Reserved.


#include "Gameplay/CombatDamageableBox.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/Component/UWitherAbilitySystemComponent.h"
#include "Data/AttributeSets/UGSAttributeSetBase.h"
#include "Engine/World.h"

ACombatDamageableBox::ACombatDamageableBox()
{
	PrimaryActorTick.bCanEverTick = false;

	// create the mesh
	RootComponent = Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	// set the collision properties
	Mesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	// enable physics
	Mesh->SetSimulatePhysics(true);

	// disable navigation relevance so boxes don't affect NavMesh generation
	Mesh->bNavigationRelevant = false;

	//Setup Ability System Component
	AbilitySystemComponent = CreateDefaultSubobject<UWitherAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetIsReplicated(true);
	}
}

void ACombatDamageableBox::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void ACombatDamageableBox::RemoveFromLevel()
{
	// destroy this actor
	Destroy();
}

void ACombatDamageableBox::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// clear the death timer
	GetWorld()->GetTimerManager().ClearTimer(DeathTimer);
}

/*
void ACombatDamageableBox::ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse)
{
	// only process damage if we still have HP
	if (CurrentHP > 0.0f)
	{
		// apply the damage
		CurrentHP -= Damage;

		// are we dead?
		if (CurrentHP <= 0.0f)
		{
			HandleDeath();
		}

		// apply a physics impulse to the box, ignoring its mass
		Mesh->AddImpulseAtLocation(DamageImpulse * Mesh->GetMass(), DamageLocation);

		// call the BP handler to play effects, etc.
		OnBoxDamaged(DamageLocation, DamageImpulse);
	}
}
*/

void ACombatDamageableBox::HandleDamageTaken(float DamageAmount, const FGameplayTagContainer& SourceTags)
{
	const UGSAttributeSetBase* BaseSet = Cast<UGSAttributeSetBase>(AbilitySystemComponent->GetAttributeSet(UGSAttributeSetBase::StaticClass()));
	if (BaseSet)
	{

		float CurrentHealth = BaseSet->GetHealth();
		float MaxHealth = BaseSet->GetMaxHealth();


		if (DamageAmount > 0.0f)
		{
			if (CurrentHealth <= 0.0f)
			{
				// die
				HandleDeath();
			}
		}
	}
}

void ACombatDamageableBox::HandleDeath()
{
	// change the collision object type to Visibility so we ignore most interactions but still retain physics collisions
	Mesh->SetCollisionObjectType(ECC_Visibility);

	// call the BP handler to play effects, etc.
	OnBoxDestroyed();

	// set up the death cleanup timer
	GetWorld()->GetTimerManager().SetTimer(DeathTimer, this, &ACombatDamageableBox::RemoveFromLevel, DeathDelayTime);
}

/*
void ACombatDamageableBox::ApplyHealing(float Healing, AActor* Healer)
{
	// stub
}
*/

void ACombatDamageableBox::NotifyDanger(const FVector& DangerLocation, AActor* DangerSource)
{
	// stub
}

bool ACombatDamageableBox::IsAlive() const
{
	return AbilitySystemComponent->IsAlive();
}

UAbilitySystemComponent* ACombatDamageableBox::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

