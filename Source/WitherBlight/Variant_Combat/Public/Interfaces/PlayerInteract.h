

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInteract.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable)
class UPlayerInteract : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WITHERBLIGHT_API IPlayerInteract
{
	GENERATED_BODY()

public:
	
	/* What happensthe player interacts with this object */
	virtual void OnInteract() = 0;
};
