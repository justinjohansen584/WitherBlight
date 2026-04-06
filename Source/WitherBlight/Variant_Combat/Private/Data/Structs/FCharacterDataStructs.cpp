// Fill out your copyright notice in the Description page of Project Settings.

#include "Data/Structs/FCharacterDataStructs.h"
#include "GameplayTagContainer.h"

float FCharacterStats::GetValueByTag(const FGameplayTag& Tag) const
{
    // 1. Get the "Leaf" name of the tag (e.g., "Stat.Health" becomes "Health")
    // Note: This logic depends on your tag naming matching your variable names!
    FString TagString = Tag.GetTagName().ToString();
    FString VarName;

    // VarName is now "Health"
    if (!TagString.Split(TEXT("."), nullptr, &VarName, ESearchCase::IgnoreCase, ESearchDir::FromEnd))
    {
        // VarName is now "Health"
        VarName = TagString;
    }

    // 2. Access the Reflection Data for this struct
    UScriptStruct* StructHandle = FCharacterStats::StaticStruct();

    // 3. Find the property by the parsed name
    FProperty* Prop = StructHandle->FindPropertyByName(*VarName);

    if (FFloatProperty* FloatProp = CastField<FFloatProperty>(Prop))
    {
        // 4. Return the value from 'this' instance
        return FloatProp->GetPropertyValue_InContainer(this);
    }

    return 0.0f; // Return default if not found
}