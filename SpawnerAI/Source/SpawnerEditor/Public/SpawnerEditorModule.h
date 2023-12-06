#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSpawnerEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};