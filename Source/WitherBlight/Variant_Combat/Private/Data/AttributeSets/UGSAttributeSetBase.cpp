
#include "Data\AttributeSets\UGSAttributeSetBase.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "CombatPlayerController.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "Data\Execution\UGSDamageExecutionCalc.h"
#include "Interfaces\CombatDamageable.h"
#include "AttributeSet.h"

DEFINE_LOG_CATEGORY(LogAttributeSet);

UGSAttributeSetBase::UGSAttributeSetBase()
{
	//stub
}

void UGSAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// This is called whenever attributes change, so for max health/mana we want to scale the current totals to match
	Super::PreAttributeChange(Attribute, NewValue);

	// If a Max value changes, adjust current to keep Current % of Current to Max
	if (Attribute == GetMaxHealthAttribute()) // GetMaxHealthAttribute comes from the Macros defined at the top of the header
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		AdjustAttributeForMaxChange(Mana, MaxMana, NewValue, GetManaAttribute());
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		AdjustAttributeForMaxChange(Stamina, MaxStamina, NewValue, GetStaminaAttribute());
	}
	else if (Attribute == GetMoveSpeedAttribute())
	{
		// Cannot slow less than 0 units/s and cannot boost more than 1000 units/s
		NewValue = FMath::Clamp<float>(NewValue, 0, 1000);
	}
}

void UGSAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	FGameplayTagContainer SpecAssetTags;
	Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

	// Get the Target actor, which should be our owner
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	ACharacter* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<ACharacter>(TargetActor);
	}

	// Get the Source actor
	AActor* SourceActor = nullptr;
	AController* SourceController = nullptr;
	ACharacter* SourceCharacter = nullptr;
	if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
	{
		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
		SourceController = Source->AbilityActorInfo->PlayerController.Get();
		if (SourceController == nullptr && SourceActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(SourceActor))
			{
				SourceController = Pawn->GetController();
			}
		}

		// Use the controller to find the source pawn
		if (SourceController)
		{
			SourceCharacter = Cast<ACharacter>(SourceController->GetPawn());
		}
		else
		{
			SourceCharacter = Cast<ACharacter>(SourceActor);
		}

		// Set the causer actor based on context if it's set
		if (Context.GetEffectCauser())
		{
			SourceActor = Context.GetEffectCauser();
		}
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// I want damage to only be handled by Execution at this point so I am going to throw an error
		const float LocalDamageDone = GetDamage();
		if (LocalDamageDone > 0.0f)
		{
			const UGameplayEffect* GE = Data.EffectSpec.Def;
			if (!GE) return;
			// 2. Look for your specific Execution Calculation class
			for (const FGameplayEffectExecutionDefinition& ExecDef : GE->Executions)
			{
				if (ExecDef.CalculationClass == UGSDamageExecutionCalc::StaticClass())
				{
					SetDamage(0.f);
					// If character was alive before damage is added, handle damage
					// This prevents damage being added to dead things and replaying death animations
					bool WasAlive = true;

					if (TargetCharacter)
					{
						WasAlive = Cast<ICombatDamageable>(TargetCharacter)->IsAlive();
					}

					if (!WasAlive)
					{
						//UE_LOG(LogTemp, Warning, TEXT("%s() %s is NOT alive when receiving damage"), *FString(__FUNCTION__), *TargetCharacter->GetName());
					}

					// Apply the damage to shield first if it exists
					const float OldShield = GetShield();
					float DamageAfterShield = LocalDamageDone - OldShield;
					if (OldShield > 0)
					{
						float NewShield = OldShield - LocalDamageDone;
						SetShield(FMath::Clamp<float>(NewShield, 0.0f, GetMaxShield()));
					}

					if (DamageAfterShield > 0)
					{
						// Apply the health change and then clamp it
						const float NewHealth = GetHealth() - DamageAfterShield;
						SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
					}

					if (TargetCharacter && WasAlive)
					{
						// This is the log statement for damage received. Turned off for live games.
						//UE_LOG(LogTemp, Log, TEXT("%s() %s Damage Received: %f"), *FString(__FUNCTION__), *GetOwningActor()->GetName(), LocalDamageDone);

						// Show damage number for the Source player unless it was self damage
						if (SourceActor != TargetActor)
						{
							ACombatPlayerController* PC = Cast<ACombatPlayerController>(SourceController);
							if (PC)
							{
								FGameplayTagContainer DamageNumberTags;

								//if (Data.EffectSpec.DynamicAssetTags.HasTag(HeadShotTag))
								//{
								//	DamageNumberTags.AddTagFast(HeadShotTag);
								//}

								//PC->ShowDamageNumber(LocalDamageDone, TargetCharacter, DamageNumberTags);
							}
						}

						//Call the HandleDamageTakenFunction
						ICombatDamageable* Interface = Cast<ICombatDamageable>(Data.Target.GetAvatarActor());
						if (Interface)
						{
							// Extract what you need from the GAS struct and pass it as primitives
							Interface->HandleDamageTaken(DamageAfterShield, *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags());
						}

						if (!Cast<ICombatDamageable>(TargetCharacter)->IsAlive())
						{
							// TargetCharacter was alive before this damage and now is not alive, give XP and Gold bounties to Source.
							// Don't give bounty to self.
							if (SourceController != TargetController)
							{
								// Create a dynamic instant Gameplay Effect to give the bounties
								UGameplayEffect* GEBounty = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("Bounty")));
								GEBounty->DurationPolicy = EGameplayEffectDurationType::Instant;

								int32 Idx = GEBounty->Modifiers.Num();
								GEBounty->Modifiers.SetNum(Idx + 2);

								FGameplayModifierInfo& InfoXP = GEBounty->Modifiers[Idx];
								InfoXP.ModifierMagnitude = FScalableFloat(GetXPBounty());
								InfoXP.ModifierOp = EGameplayModOp::Additive;
								InfoXP.Attribute = UGSAttributeSetBase::GetXPAttribute();

								FGameplayModifierInfo& InfoGold = GEBounty->Modifiers[Idx + 1];
								InfoGold.ModifierMagnitude = FScalableFloat(GetGoldBounty());
								InfoGold.ModifierOp = EGameplayModOp::Additive;
								InfoGold.Attribute = UGSAttributeSetBase::GetGoldAttribute();

								Source->ApplyGameplayEffectToSelf(GEBounty, 1.0f, Source->MakeEffectContext());
							}
						}
					}
				}
			}
		}
	}// Damage
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Handle other health changes.
		// Health loss should go through Damage.
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	} // Health
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		// Handle mana changes.
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	} // Mana
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		// Handle stamina changes.
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
	}
	else if (Data.EvaluatedData.Attribute == GetShieldAttribute())
	{
		// Handle shield changes.
		SetShield(FMath::Clamp(GetShield(), 0.0f, GetMaxShield()));
	}
}

void UGSAttributeSetBase::ResetHealth()
{
	// Get the actual numerical value from the MaxHealth attribute data
	float MaxHealthValue = MaxHealth.GetCurrentValue();
	// Set the Base Value of Health to match that number
	SetHealth(MaxHealthValue);
}


void UGSAttributeSetBase::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		// Change current value to maintain the current Val / Max percent
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

