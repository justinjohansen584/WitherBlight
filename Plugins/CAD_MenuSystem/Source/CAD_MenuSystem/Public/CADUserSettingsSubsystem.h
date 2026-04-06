// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CADUserSettingsSubsystem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class CAD_MENUSYSTEM_API UCADUserSettingsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/*
	UFUNCTION(BlueprintCallable, Category = "Video Settings")
	TArray<struct FCADDisplayInfo> GetAllDisplayInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Video Settings")
	void SetActiveDisplayByID(const FString& MonitorID);
	*/
};
