


#include "LevelManager.h"
#include "Gameplay\Environment\ChestLootManager.h"


// Sets default values
ALevelManager::ALevelManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelManager::BeginPlay()
{
	Super::BeginPlay();
	UChestLootManager* Sub = GetWorld()->GetSubsystem<UChestLootManager>();
	if (Sub)
	{
		Sub->SetLootTableAndInitialize(LootDataAsset);
	}
}



