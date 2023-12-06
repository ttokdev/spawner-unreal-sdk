// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "SpawnerTypes.h"

class FSpawnerAIModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
private:
	FSpawnerWorkspaceData WorkspaceData;
public:
	FORCEINLINE const FSpawnerWorkspaceData& GetWorkspaceData() { return WorkspaceData; }
	FORCEINLINE void SetWorkspaceData(const FSpawnerWorkspaceData& Data) { WorkspaceData = Data; }
};
