// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "SpawnerTypes.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSpawnerAI, Log, All);

class FSpawnerAIModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	bool ShouldInitializeApi() { return IsEnabledApi; }
	void SetIsEnabledApi(bool bIsEnabled) { IsEnabledApi = bIsEnabled; }

	const FSpawnerWorkspaceData& GetWorkspaceData() { return WorkspaceData; }
	void SetWorkspaceData(const FSpawnerWorkspaceData& Data) { WorkspaceData = Data; }

private:
	bool IsEnabledApi = true;
	FSpawnerWorkspaceData WorkspaceData;
};
