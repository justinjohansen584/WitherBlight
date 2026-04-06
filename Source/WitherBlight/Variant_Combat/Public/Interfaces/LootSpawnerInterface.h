

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LootSpawnerInterface.generated.h"

struct FLootSpawnersStucts;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable)
class ULootSpawnerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WITHERBLIGHT_API ILootSpawnerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	virtual void InitializeAndSpawn(FLootSpawnersStucts LootSpawnInfo) = 0;

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	virtual void Interact() = 0;
	
};
