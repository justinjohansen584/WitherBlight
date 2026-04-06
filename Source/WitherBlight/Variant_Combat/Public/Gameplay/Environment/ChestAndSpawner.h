

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces\LootSpawnerInterface.h"
#include "Data\Structs\FLootSpawnersStucts.h"
#include "Interfaces/PlayerInteract.h"
#include "ChestAndSpawner.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogChest, Log, All);

UCLASS()
class WITHERBLIGHT_API AChestAndSpawner : public AActor, public ILootSpawnerInterface, public IPlayerInteract
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UGeometryCollectionComponent> GeometryCollectionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereColliderComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UFieldSystemComponent> FieldSystemComponent;


public:	
	// Sets default values for this actor's properties
	AChestAndSpawner();

	virtual void Tick(float DeltaTime) override;

	/** Begining of ILootSpawnerInterface*/
	virtual void InitializeAndSpawn(FLootSpawnersStucts LootSpawnInfo) override;
	virtual void Interact() override;
	/** End of ILootSpawnerInterface*/

	/* Start of IPlayerInteract Interface */

	/* What happens when the Item is picked up */
	virtual void OnInteract() override;

	/* End of IPlayerInteract Interface */

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	virtual void VerifyEditorConstructor();
	
private:
	FLootSpawnersStucts ChestSpawnInfo;

public:
	// --- Physics Breath Settings ---
	UPROPERTY(EditAnywhere, Category = "Breathing")
	float BreathFrequency = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Breathing")
	float PushStrength = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Breathing")
	float ExtraNegative = -.02;

private:

	UPROPERTY(VisibleAnywhere, Category = "Breathing")
	float CurrentTime = 0.0f;

};
