

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IItemsAbility.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI, NotBlueprintable)
class UItemsAbility : public UInterface
{
	GENERATED_BODY()
};

class IItemsAbility
{
	GENERATED_BODY()

public:

	/* What happens when the Item is picked up */
	virtual void OnPickup() = 0;

};