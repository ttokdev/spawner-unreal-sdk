// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerEditorApiSubsystem.h"
#include "SpawnerEditorModule.h"

const FSpawnerWorkspaceData& USpawnerEditorApiSubsystem::GetCachedWorkspaceData() const
{
	FSpawnerEditorModule* Module = static_cast<FSpawnerEditorModule*>(FModuleManager::Get().GetModule("SpawnerEditor"));
	if (ensure(Module))
	{
		return Module->GetEditorWorkspaceData();
	}
	else
	{
		static FSpawnerWorkspaceData Data;
		return Data;
	}
}

void USpawnerEditorApiSubsystem::SetCachedWorkspaceData(const FSpawnerWorkspaceData& Data)
{
	FSpawnerEditorModule* Module = static_cast<FSpawnerEditorModule*>(FModuleManager::Get().GetModule("SpawnerEditor"));
	if (ensure(Module))
	{
		Module->SetEditorWorkspaceData(Data);
		return;
	}
	else
	{
		return;
	}
}

bool USpawnerEditorApiSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Editor;
}

void USpawnerEditorApiSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USpawnerEditorApiSubsystem::Deinitialize()
{
	Super::Deinitialize();
}
