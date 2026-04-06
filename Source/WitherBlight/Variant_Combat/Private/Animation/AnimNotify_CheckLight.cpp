// Copyright Epic Games, Inc. All Rights Reserved.


#include "Animation/AnimNotify_CheckLight.h"
#include "Interfaces/CombatAttacker.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_CheckLight::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// cast the owner to the attacker interface
	if (ICombatAttacker* AttackerInterface = Cast<ICombatAttacker>(MeshComp->GetOwner()))
	{
		// tell the actor to check for Light string
		AttackerInterface->CheckLight();
	}
}

FString UAnimNotify_CheckLight::GetNotifyName_Implementation() const
{
	return FString("Check Light String");
}
