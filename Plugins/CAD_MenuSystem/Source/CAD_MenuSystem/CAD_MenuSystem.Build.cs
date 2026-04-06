using UnrealBuildTool;

public class CAD_MenuSystem : ModuleRules
{
	public CAD_MenuSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		// Core dependencies required for basic C++ functionality
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
            "Slate",
            "SlateCore",
            "ApplicationCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}