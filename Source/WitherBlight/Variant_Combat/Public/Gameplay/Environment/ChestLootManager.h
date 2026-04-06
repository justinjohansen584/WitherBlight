

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Data\Structs\FLootSpawnersStucts.h"
#include "ChestLootManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogChestLoot, Log, All);


/**
 * 
 */
UCLASS()
class WITHERBLIGHT_API UChestLootManager : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    // Called when the subsystem is created
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    // Called when the subsystem is destroyed
    virtual void Deinitialize() override;


    virtual void OnWorldBeginPlay(UWorld& InWorld) override;

    virtual void SetLootTableAndInitialize(class ULootManagerDataAsset* NewTable);

    virtual void SpawnChest();

    /*
    * TO DO :
    * this is where you would take into acount
    */
    virtual FLootSpawnersStucts RoleRarityAndReturnItem();

protected:

    TArray<FLootSpawnersStucts*> AllPossibleItems;
    TArray<FLootSpawnersStucts*> CommonPossibleItems;
    TArray<FLootSpawnersStucts*> UncommonPossibleItems;
    TArray<FLootSpawnersStucts*> RarePossibleItems;
    TArray<FLootSpawnersStucts*> EpicPossibleItems;
    TArray<FLootSpawnersStucts*> LegendaryPossibleItems;
    TArray<FLootSpawnersStucts*> UniquePossibleItems;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Setup")
    TObjectPtr<ULootManagerDataAsset> LootManagerDataAsset;

};
