

#include "Gameplay/Component/BlockGatherShootComponent.h"
#include "TimerManager.h"
#include "CombatCharacter.h"
#include "Data/Structs/FBlockDataStructs.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Gameplay/Environment/EnviromentalBase.h"

DEFINE_LOG_CATEGORY(LogBlockGather);

// Sets default values for this component's properties
UBlockGatherShootComponent::UBlockGatherShootComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UBlockGatherShootComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		UE_LOG(LogBlockGather, Error, TEXT("Owner actor for Component <%s> is Null."), *GetName());
		return;
	}
	OwnerCombatAttacker = Cast<ICombatAttacker>(OwnerActor);
	if (!OwnerCombatAttacker)
	{
		UE_LOG(LogBlockGather, Error, TEXT("Owner does not include ICombatAttacker Interface: actor name <%s>"), *GetName());
		return;
	}
	FTimerDelegate TimerDel;
	// BINDING WITH PAYLOAD
	TimerDel.BindUFunction(this, FName("AttemptToGatherBlocks"));
	// Set timer to call CustomUpdate every frame (0.0f)
	OwnerActor->GetWorldTimerManager().SetTimer(GatherTimerHandle, TimerDel, 2.50f, true);
	
	RandStream.Initialize(0);

	OnBlockGathered.AddDynamic(this, &UBlockGatherShootComponent::BlockGathered);

}

int32 UBlockGatherShootComponent::GetTotalGatheredBlocks()
{
	return TotalGatheredBlocks;
}

void UBlockGatherShootComponent::BlockGathered(FBlockInfo blockInfo)
{
	TotalGatheredBlocks += blockInfo.Worth;
	OwnerCombatAttacker->BlockGathered(TotalGatheredBlocks);
}

void UBlockGatherShootComponent::AttemptToGatherBlocks()
{
	FVector CenterLocation = OwnerActor->GetActorLocation();
	TArray<AActor*> OverlappedActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // What types of objects to check (WorldStatic, Pawn, PhysicsBody, etc.)
	AActor* IgnoreActor = OwnerActor;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(IgnoreActor);

	// 1. Perform the Overlap Query
	bool bSuccess = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		CenterLocation,
		OwnerCombatAttacker->GetCharacterStats().BlockTraceRadius,
		ObjectTypes,
		AActor::StaticClass(), // Filter by a base class (AActor is usually sufficient for the overlap)
		ActorsToIgnore,
		OverlappedActors
	);

	TArray<AEnviromentalBase*> FoundObjects;
	for (AActor* Actor : OverlappedActors)
	{
		if (AEnviromentalBase* Target = Cast<AEnviromentalBase>(Actor))
		{
			FoundObjects.Add(Target);
		}
	}
	for (AEnviromentalBase* obj : FoundObjects)
	{
		float RandomSeed = RandStream.FRandRange(0.0f, 1.0f);
		if (OwnerCombatAttacker->GetCharacterStats().fChanceOfBlockGrab > RandomSeed)
		{
			obj->GrabBlock(this);
		}
	}

}


//TO DO: Need to do this on Death or remove
//OnBlockGathered.RemoveDynamic(this, &ACombatCharacter::BlockGathered);


