

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CADDisplayInfo.generated.h"

USTRUCT(BlueprintType)
struct CAD_MENUSYSTEM_API FCADDisplayInfo
{
	GENERATED_BODY()
		
	UPROPERTY(BlueprintReadOnly)
	FString Name = FString("");
	UPROPERTY(BlueprintReadOnly)
	FString ID = FString("");
	UPROPERTY(BlueprintReadOnly)
	int32 NativeWidth = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 NativeHeight = 0;
	UPROPERTY(BlueprintReadOnly)
	FIntPoint MaxResolution = FIntPoint(ForceInitToZero);
	UPROPERTY(BlueprintReadOnly)
	bool bIsPrimary = false;
	UPROPERTY(BlueprintReadOnly)
	int32 DPI = 0;

	FCADDisplayInfo()
	{
		Name = FString("");
		ID = FString("");
		NativeWidth = 0;
		NativeHeight = 0;
		MaxResolution = FIntPoint(ForceInitToZero);\
		bIsPrimary = false;
		DPI = 0;
	}

	FCADDisplayInfo(const FString& InName, const FString& InID, int32 InNativeWidth, int32 InNativeHeight,
		const FIntPoint& InMaxResolution, bool InIsPrimary, int32 InDPI)
		: Name(InName),
		ID(InID),
		NativeWidth(InNativeWidth),
		NativeHeight(InNativeHeight),
		MaxResolution(InMaxResolution),
		bIsPrimary(InIsPrimary),
		DPI(InDPI)
	{
	}
};
