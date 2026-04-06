

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
//#include "Gameplay\Environment\ChestAndSpawner.h"
#include "LootManagerDataAsset.generated.h"
/**
 * 
 */
UCLASS(BlueprintType)
class WITHERBLIGHT_API ULootManagerDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChestSpawner")
	TArray<TSubclassOf<class AChestAndSpawner>> ChestClasses = {};
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Setup|ChestLootManager")
	TObjectPtr<UDataTable> AllItemsDataTable;
};
