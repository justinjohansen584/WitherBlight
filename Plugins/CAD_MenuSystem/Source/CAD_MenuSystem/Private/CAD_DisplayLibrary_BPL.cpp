
#include "CAD_DisplayLibrary_BPL.h"
#include "CADUserSettingsSubsystem.h"
#include "GenericPlatform/GenericApplication.h"
#include "Widgets/SWindow.h"
#include "GameFramework/GameUserSettings.h"
#include "CADDisplayInfo.h"

TArray<FCADDisplayInfo> UCAD_DisplayLibrary_BPL::GetAllDisplayInfo()
{

	TArray<FCADDisplayInfo> MonitorInfoArray;

	FDisplayMetrics DisplayMetrics;
	FDisplayMetrics::RebuildDisplayMetrics(DisplayMetrics);

	for (const FMonitorInfo& MonitorInfo : DisplayMetrics.MonitorInfo)
	{
		MonitorInfoArray.Add(FCADDisplayInfo(MonitorInfo.Name, MonitorInfo.ID, MonitorInfo.NativeWidth, MonitorInfo.NativeHeight, MonitorInfo.MaxResolution, MonitorInfo.bIsPrimary, MonitorInfo.DPI));
	}
	return MonitorInfoArray;
}

bool UCAD_DisplayLibrary_BPL::SetActiveDisplayByID(int32 DisplayIndex)
{
	FDisplayMetrics Displays;
	FDisplayMetrics::RebuildDisplayMetrics(Displays);

	if (DisplayIndex >= Displays.MonitorInfo.Num())
	{
		// Non existing display
		return false;
	}

	const FMonitorInfo TargetMonitor = Displays.MonitorInfo[DisplayIndex];
	//TargetMonitor.WorkArea.
	FVector2D WindowPosition(static_cast<float>(TargetMonitor.WorkArea.Left), static_cast<float>(TargetMonitor.WorkArea.Top));

	if (GEngine && GEngine->GameViewport)
	{
		// Display switching
		TSharedPtr<SWindow> GWindow = GEngine->GameViewport->GetWindow();
		GWindow->MoveWindowTo(WindowPosition);

		//Setting resolution
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		UserSettings->SetScreenResolution(FIntPoint(TargetMonitor.NativeWidth, TargetMonitor.NativeHeight));
		UserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
		UserSettings->ApplySettings(true);
		UserSettings->ApplyResolutionSettings(false);
		return true;
	}

	return false;
}

int UCAD_DisplayLibrary_BPL::GetDisplayCount()
{
	FDisplayMetrics DisplayMetrics;
	FDisplayMetrics::RebuildDisplayMetrics(DisplayMetrics);
	return DisplayMetrics.MonitorInfo.Num();
}
