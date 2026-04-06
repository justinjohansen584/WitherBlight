#include "CAD_MenuSystem.h"

#define LOCTEXT_NAMESPACE "FCAD_MenuSystemModule"

void FCAD_MenuSystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory
}

void FCAD_MenuSystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCAD_MenuSystemModule, CAD_MenuSystem)