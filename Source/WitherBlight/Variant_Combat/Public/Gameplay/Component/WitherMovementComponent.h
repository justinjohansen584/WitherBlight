#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WitherMovementComponent.generated.h"

/**
 *
 */
UCLASS()
class WITHERBLIGHT_API UWitherMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UWitherMovementComponent(const FObjectInitializer& ObjectInitializer);
	//~UMovementComponent interface
	virtual float GetMaxSpeed() const override;
	//~End of UMovementComponent interface

};