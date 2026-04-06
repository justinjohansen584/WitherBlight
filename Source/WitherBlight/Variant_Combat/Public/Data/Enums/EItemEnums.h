

#pragma once

#include "EItemEnums.generated.h"

UENUM(BlueprintType) // Exposes the enum to Blueprints
enum class EItemPickupType : uint8
{
    // Enum values start from 0 by default, or you can assign explicit values
    Projectile_Impact    UMETA(DisplayName = "Projectile Impact"),
    Projectile_Fire      UMETA(DisplayName = "Projectile Fire"),
    Body                 UMETA(DisplayName = "Body"),
    Stats                UMETA(DisplayName = "Stats"),
    Other                UMETA(DisplayName = "Other"),
    EMS_MAX              UMETA(Hidden) // Optional: a common practice for internal max value
};
