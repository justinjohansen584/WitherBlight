

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CAD_DisplayLibrary_BPL.generated.h"


/**
 * 
 */
UCLASS()
class CAD_MENUSYSTEM_API UCAD_DisplayLibrary_BPL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "Display Library")
	static int GetDisplayCount();

	UFUNCTION(BlueprintCallable, Category = "Display Library")
	static TArray<struct FCADDisplayInfo> GetAllDisplayInfo();

	UFUNCTION(BlueprintCallable, Category = "Display Library")
	static bool SetActiveDisplayByID(int32 DisplayIndex);
	
	
};
