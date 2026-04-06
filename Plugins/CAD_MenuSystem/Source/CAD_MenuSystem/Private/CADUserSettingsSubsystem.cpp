// Fill out your copyright notice in the Description page of Project Settings.


#include "CADUserSettingsSubsystem.h"
/*
#include "GenericPlatform/GenericApplication.h"
#include "Widgets/SWindow.h"
#include "GameFramework/GameUserSettings.h"
#include "CADDisplayInfo.h"

TArray<FCADDisplayInfo> UCADUserSettingsSubsystem::GetAllDisplayInfo() const
{

	TArray<FCADDisplayInfo> MonitorInfoArray;

	FDisplayMetrics DisplayMetrics;
	FDisplayMetrics::RebuildDisplayMetrics(DisplayMetrics);

	for (const FMonitorInfo& MonitorInfo: DisplayMetrics.MonitorInfo)
	{
		MonitorInfoArray.Add(FCADDisplayInfo(MonitorInfo.Name, MonitorInfo.ID, MonitorInfo.NativeWidth, MonitorInfo.NativeHeight, MonitorInfo.MaxResolution, MonitorInfo.bIsPrimary, MonitorInfo.DPI));
	}
	return MonitorInfoArray;
}

void UCADUserSettingsSubsystem::SetActiveDisplayByID(const FString& MonitorID)
{
	FDisplayMetrics DisplayMetrics;
	FDisplayMetrics::RebuildDisplayMetrics(DisplayMetrics);

	for (const FMonitorInfo& MonitorInfo : DisplayMetrics.MonitorInfo)
	{
		if (MonitorInfo.ID == MonitorID)
		{
			UE_LOG(LogTemp, Log, TEXT("Setting active display to ID: %s"), *MonitorID);

			FVector2D NewWindowPosition(MonitorInfo.WorkArea.Left, MonitorInfo.WorkArea.Top);

			if (GEngine && GEngine->GameViewport)
			{
				TSharedPtr<SWindow> GWindow = GEngine->GameViewport->GetWindow();

				if (GWindow.IsValid())
				{
					// 1. Force Windowed mode first
					UGameUserSettings* Settings = GEngine->GetGameUserSettings();
					Settings->SetFullscreenMode(EWindowMode::Windowed);
					Settings->ApplyNonResolutionSettings();

					// 2. Perform your MoveWindowTo logic (your current code)
					GWindow->MoveWindowTo(NewWindowPosition);

					// 3. (Optional) Switch back to WindowedFullscreen on the new monitor
					Settings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
					Settings->ApplyNonResolutionSettings();
					UE_LOG(LogTemp, Log, TEXT("Moved window to position: (%f, %f)"), NewWindowPosition.X, NewWindowPosition.Y);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("GameViewport window is not valid."));
				}
			}

			return;
		}
	}
}
*/
