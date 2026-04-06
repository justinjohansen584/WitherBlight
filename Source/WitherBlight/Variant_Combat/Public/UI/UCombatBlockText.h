

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UCombatBlockText.generated.h"

/**
 *  A basic text user widget to display block count.
 */
UCLASS(abstract)
class UCombatBlockText : public UUserWidget
{
	GENERATED_BODY()
public:
	/** Increase block amount*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Block Text")
	void ChangeTextAmount(int32 AmountToChange);

};
