

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Structs/FBlockDataStructs.h"
#include "BlockGatherShootComponent.generated.h"

class ICombatAttacker;

DECLARE_LOG_CATEGORY_EXTERN(LogBlockGather, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBlockGatheredDelegate, FBlockInfo, blockInfo);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UBlockGatherShootComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBlockGatherShootComponent();

	UFUNCTION(BlueprintCallable, Category = "Helper")
	int32 GetTotalGatheredBlocks();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	/** Delgate Called when ever a block is gathered*/
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FBlockGatheredDelegate OnBlockGathered;

protected:

	UFUNCTION(BlueprintCallable, Category = "Gather")
	virtual void AttemptToGatherBlocks();


	/** Handles charged attack released from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Events")
	virtual void BlockGathered(FBlockInfo blockInfo);


protected:

	UPROPERTY(VisibleAnywhere, Category = "Gather")
	int32 TotalGatheredBlocks = 0;
	

	/** Random Stream **/
	FRandomStream RandStream;

	/* Frequency of Gather Timer */
	FTimerHandle GatherTimerHandle;

	ICombatAttacker* OwnerCombatAttacker;

	TObjectPtr<AActor> OwnerActor;


};
