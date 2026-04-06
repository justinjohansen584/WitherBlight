

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelManager.generated.h"

UCLASS()
class WITHERBLIGHT_API ALevelManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Setup|ChestLootManager")
	//TObjectPtr<UDataTable> AllItemsDataTable;	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Setup|ChestLootManager")
	TObjectPtr<class ULootManagerDataAsset> LootDataAsset;	
};
