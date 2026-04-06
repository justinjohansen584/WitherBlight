#pragma once

#include "Modules/ModuleManager.h"

class FCAD_MenuSystemModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};