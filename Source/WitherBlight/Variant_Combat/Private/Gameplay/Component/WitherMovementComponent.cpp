


#include "Gameplay\Component\WitherMovementComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework\PlayerState.h"
#include "Data\AttributeSets\UGSAttributeSetBase.h"
#include "CombatPlayerState.h"


UWitherMovementComponent::UWitherMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

float UWitherMovementComponent::GetMaxSpeed() const
{
	ACombatPlayerState* PS = (GetPawnOwner()->GetPlayerState<ACombatPlayerState>());
	if (UAbilitySystemComponent* ASC =  Cast<IAbilitySystemInterface>(PS)->GetAbilitySystemComponent())
	{
		if (MovementMode == MOVE_Walking)
		{
			/*
			if (ASC->HasMatchingGameplayTag(TAG_Gameplay_MovementStopped))
			{
				return 0;
			}
			*/

			const float MaxSpeedFromAttribute = PS->GetMoveSpeed();
			if (MaxSpeedFromAttribute > 0.0f)
			{
				return MaxSpeedFromAttribute;
			}
		}
	}

	return Super::GetMaxSpeed();
}