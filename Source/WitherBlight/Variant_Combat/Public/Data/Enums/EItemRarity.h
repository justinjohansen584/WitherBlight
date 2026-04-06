

#pragma once

#include "EItemRarity.generated.h"


UENUM(BlueprintType) // Exposes the enum to Blueprints
enum class EItemRarity : uint8
{
	// Enum values start from 0 by default, or you can assign explicit values
	Common         UMETA(DisplayName = "Common"),
	Uncommon       UMETA(DisplayName = "Uncommon"),
	Rare           UMETA(DisplayName = "Rare"),
	Epic           UMETA(DisplayName = "Epic"),
	Legendary      UMETA(DisplayName = "Legendary"),
	Unique         UMETA(DisplayName = "Unique"),
	EMS_MAX        UMETA(Hidden) // Optional: a common practice for internal max value
};