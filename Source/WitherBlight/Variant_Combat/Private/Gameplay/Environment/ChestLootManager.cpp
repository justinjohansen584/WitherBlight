


#include "Gameplay\Environment\ChestLootManager.h"
#include "Gameplay\Environment\ChestAndSpawner.h"
#include "Gameplay\Items\ItemPickUp.h"
#include "Data\DataAsset\LootManagerDataAsset.h"
#include "NavigationSystem.h"

DEFINE_LOG_CATEGORY(LogChestLoot);

void UChestLootManager::Initialize(FSubsystemCollectionBase& Collection)
{

}


void UChestLootManager::SetLootTableAndInitialize(ULootManagerDataAsset* LootDataAsset)
{ 
    LootManagerDataAsset = LootDataAsset;
    LootManagerDataAsset->AllItemsDataTable->GetAllRows(TEXT("Getting All ItemStats"), AllPossibleItems);
    for(FLootSpawnersStucts* LootData : AllPossibleItems)
    {
        if (LootData->Rarity == EItemRarity::Common)
        {
            CommonPossibleItems.Add(LootData);
        }
        else if (LootData->Rarity == EItemRarity::Uncommon)
        {
            UncommonPossibleItems.Add(LootData);
        }
        else if (LootData->Rarity == EItemRarity::Rare)
        {
            RarePossibleItems.Add(LootData);
        }
        else if (LootData->Rarity == EItemRarity::Epic)
        {
            EpicPossibleItems.Add(LootData);
        }
        else if (LootData->Rarity == EItemRarity::Legendary)
        {
            LegendaryPossibleItems.Add(LootData);
        }
        else if (LootData->Rarity == EItemRarity::Unique)
        {
            UniquePossibleItems.Add(LootData);;
        }   
    }
    SpawnChest(); //This is for testing
}

void UChestLootManager::Deinitialize()
{
}



void UChestLootManager::OnWorldBeginPlay(UWorld& InWorld)
{
    Super::OnWorldBeginPlay(InWorld);
}

void UChestLootManager::SpawnChest()
{
    if (AllPossibleItems.Num() > 0)
    {
        // this is bad and should be a different way of getting chest class
        int32 RandomIndex = FMath::RandRange(0, AllPossibleItems.Num() - 1);
        FLootSpawnersStucts ItemSpawnerStruct = *AllPossibleItems[RandomIndex];

        UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

        if (NavSys )
        {
            FNavLocation RandomLocation;
            bool bSuccess = NavSys->GetRandomPoint(RandomLocation);

            if (bSuccess)
            {
                // 3. Use the location
                FVector TargetPoint = RandomLocation.Location;
                RandomIndex = FMath::RandRange(0, LootManagerDataAsset->ChestClasses.Num() - 1);
                TSubclassOf<AChestAndSpawner> ChestClass = LootManagerDataAsset->ChestClasses[RandomIndex];
                if (!ChestClass)
                {
                    UE_LOG(LogChestLoot, Error, TEXT("Chest did not spawn because it did not have a valid chest class"));
                    return;
                }
                AChestAndSpawner* NewChest = GetWorld()->SpawnActorDeferred<AChestAndSpawner>(ChestClass, FTransform(TargetPoint));
                if (!NewChest)
                {
                    UE_LOG(LogChestLoot, Error, TEXT("Chest did not spawn was not valid for some reason"));
                    return;
                }
                //NewChest->InitializeAndSpawn(ItemSpawnerStruct);
                NewChest->FinishSpawning(FTransform(TargetPoint));
                return;
            }
            else
            {
                UE_LOG(LogChestLoot, Error, TEXT("Chest did not spawn because %s was not a valid location"), *(RandomLocation.Location).ToString());
            }
        }
    }
    UE_LOG(LogChestLoot, Error, TEXT("Chest to spawn at all"));
}

FLootSpawnersStucts UChestLootManager::RoleRarityAndReturnItem()
{
    FLootSpawnersStucts ItemSpawnerStruct;
    int32 RandomIndex = FMath::FRandRange(0.0f, 100.0f);
    if (UniquePossibleItems.Num()>0 && RandomIndex <= 1.0f)
    {
        RandomIndex = FMath::RandRange(0, UniquePossibleItems.Num() - 1);
        return *UniquePossibleItems[RandomIndex];
    }
    else if (LegendaryPossibleItems.Num() > 0 && RandomIndex <= 5.0f)
    {
        RandomIndex = FMath::RandRange(0, LegendaryPossibleItems.Num() - 1);
        return *LegendaryPossibleItems[RandomIndex];
    }
    else if (EpicPossibleItems.Num() > 0 && RandomIndex <= 10.0f)
    {
        RandomIndex = FMath::RandRange(0, EpicPossibleItems.Num() - 1);
        return *EpicPossibleItems[RandomIndex];
    }
    else if (RarePossibleItems.Num() > 0 && RandomIndex <= 25.0f)
    {
        RandomIndex = FMath::RandRange(0, RarePossibleItems.Num() - 1);
        return *RarePossibleItems[RandomIndex];
    }
    else if (UncommonPossibleItems.Num() > 0 && RandomIndex <= 50.0f)
    {
        RandomIndex = FMath::RandRange(0, UncommonPossibleItems.Num() - 1);
        return *UncommonPossibleItems[RandomIndex];
    }
    else if (CommonPossibleItems.Num() > 0 && RandomIndex <= 100.0f)
    {
        RandomIndex = FMath::RandRange(0, CommonPossibleItems.Num() - 1);
        return *CommonPossibleItems[RandomIndex];
    }

    RandomIndex = FMath::RandRange(0, AllPossibleItems.Num() - 1);
    return *AllPossibleItems[RandomIndex];
}