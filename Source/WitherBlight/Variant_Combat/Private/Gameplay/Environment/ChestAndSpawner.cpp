


#include "Gameplay/Environment/ChestAndSpawner.h"
#include "Components\SphereComponent.h"
#include "Data/Structs/FLootSpawnersStucts.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemObjects.h"
#include "Field\FieldSystemTypes.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Gameplay\Items\ItemPickUp.h"
//#include "Field/FieldSystemNodes.h"
#include "Gameplay\Environment\ChestLootManager.h"


DEFINE_LOG_CATEGORY(LogChest);

// Sets default values
AChestAndSpawner::AChestAndSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereColliderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereColliderComponent"));
	if (SphereColliderComponent)
	{
		RootComponent = SphereColliderComponent;
	}
	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry Component"));
	GeometryCollectionComponent->SetupAttachment(RootComponent);
	GeometryCollectionComponent->SetEnableGravity(false);

	FieldSystemComponent = CreateDefaultSubobject<UFieldSystemComponent>(TEXT("FieldSystemComponent"));

#if WITH_EDITOR
	VerifyEditorConstructor();
#endif
}

// Called when the game starts or when spawned
void AChestAndSpawner::BeginPlay()
{
	Super::BeginPlay();
	GeometryCollectionComponent->CrumbleActiveClusters();
	//GeometryCollectionComponent->SetLinearDamping(10.0f); // High resistance
	//GeometryCollectionComponent->SetAngularDamping(10.0f);
	//GeometryCollectionComponent->SetAllPhysicsSimulating(true);
	//GeometryCollectionComponent->ChaosSolverSettings.SleepingThreshold = 0.0f;
}

void AChestAndSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AChestAndSpawner::InitializeAndSpawn(FLootSpawnersStucts LootSpawnInfo)
{
	//ChestSpawnInfo = LootSpawnInfo;
}

void AChestAndSpawner::Interact()
{
	

	UWorld* World = GetWorld();
	if (World)
	{
		UChestLootManager* LootManager = World->GetSubsystem<UChestLootManager>();

		if (LootManager)
		{
			ChestSpawnInfo = LootManager->RoleRarityAndReturnItem();
		}
	}

	FVector SpawnLocation = GetActorLocation() + ChestSpawnInfo.UpOffset; // Slightly above floor
	FRotator SpawnRotation = FRotator::ZeroRotator;

	// 2. Spawn the Item
	AItemPickUp* SpawnedItem = GetWorld()->SpawnActorDeferred<AItemPickUp>(ChestSpawnInfo.ItemClass, FTransform(SpawnRotation, SpawnLocation));

	if (SpawnedItem)
	{
		// Complete the spawn so the physics body is created
		SpawnedItem->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
		UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(SpawnedItem->GetRootComponent());

		if (RootComp && RootComp->IsSimulatingPhysics())
		{
			FVector UpVector = FVector::UpVector;
			float ConeAngleDegrees = ChestSpawnInfo.ConeAngleDegrees;
			FVector RandomDir = FMath::VRandCone(UpVector, FMath::DegreesToRadians(ConeAngleDegrees));
			float LaunchStrength = ChestSpawnInfo.LaunchStrength;
			FVector Impulse = RandomDir * LaunchStrength;

			// Apply to the physics body
			RootComp->AddImpulse(Impulse, NAME_None, true);
		}
	}

	SetLifeSpan(1.0f);
}

void AChestAndSpawner::OnInteract()
{
	Interact();
}


#if WITH_EDITOR
void AChestAndSpawner::VerifyEditorConstructor()
{
	if (!SphereColliderComponent)
	{
		UE_LOG(LogChest, Error, TEXT("SETUP ERROR: Item %s's does not have SphereCollider"), *GetName());
	}
	if (!GeometryCollectionComponent)
	{
		UE_LOG(LogChest, Error, TEXT("SETUP ERROR: Item %s's does not have Geometry Collection"), *GetName());
	}
}
#endif
