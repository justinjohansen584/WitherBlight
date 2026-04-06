

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Data\Enums\EItemRarity.h"
#include "FLootSpawnersStucts.generated.h"

class AItemPickUp;
class AChestAndSpawner;

/**
 * Struct Containing all the infomation for a loot Spawner unit.
 */
USTRUCT(BlueprintType)
struct FLootSpawnersStucts : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Rarity")
	EItemRarity Rarity = EItemRarity::Common;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ChestSpawner")
	FName name = "";
	UPROPERTY(BlueprintReadWrite, Category = "ChestSpawner")
	TObjectPtr<AActor> Owner = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChestSpawner")
	TSubclassOf<AChestAndSpawner> ChestClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChestSpawner")
	TSubclassOf<AItemPickUp> ItemClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemPhysics")
	float LaunchStrength = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemPhysics")
	float ConeAngleDegrees = 75.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemPhysics")
	FVector UpOffset = { 0,0,100 };

};
