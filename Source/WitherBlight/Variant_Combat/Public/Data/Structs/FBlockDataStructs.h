

#pragma once

#include "CoreMinimal.h"
#include "FBlockDataStructs.generated.h"

class AEnviromentalBase;
struct FTimerHandle;
class UActorComponent;

/**
 * Infomation for a Gather block timer info
 */
USTRUCT(BlueprintType)
struct FBlockTimerInfo
{
	GENERATED_BODY()
	FTimerHandle TimerHandle = FTimerHandle();
	int32 index = 1;
	float elpasedTime = 0.0f;
	TObjectPtr<UActorComponent> TargetComponent = nullptr;
};
/**
 *A struct that contain all the info for a block
 */
USTRUCT(BlueprintType)
struct FBlockInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block")
	FName name = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block")
	TObjectPtr<AEnviromentalBase> Actor = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block")
	int32 Worth = 1;
	//Make this pass a package
};
