/** Copyright 2024 Warps, Inc. **/


#include "SpawnerEditorSettings.h"
#include "SpawnerAIModule.h"
#include "SpawnerPlayerComponent.h"
#include "SpawnerCharacterComponent.h"

#define LOCTEXT_NAMESPACE "USpawnerEditorSettings"

USpawnerEditorSettings::USpawnerEditorSettings()
    :Super()
{
    EnableApi(true);
    SpawnerPlayerComponent = USpawnerPlayerComponent::StaticClass();
    SpawnerCharacterComponent = USpawnerCharacterComponent::StaticClass();

    OnSettingChanged().AddUObject(this, &USpawnerEditorSettings::OnEnableApiSettingChanged);
}

void USpawnerEditorSettings::OnEnableApiSettingChanged(UObject*, struct FPropertyChangedEvent&)
{
    FSpawnerAIModule* Module = static_cast<FSpawnerAIModule*>(FModuleManager::Get().GetModule("SpawnerAI"));
    if (ensure(Module))
    {
        if (EnableSpawnerAPI)
        {
            Module->SetIsEnabledApi(true);
        }
        else
        {
            Module->SetIsEnabledApi(false);
        }

    }
}

FName USpawnerEditorSettings::GetContainerName() const
{
    return Super::GetContainerName();;
}

FName USpawnerEditorSettings::GetCategoryName() const
{
    return FName("Plugins");
}

FName USpawnerEditorSettings::GetSectionName() const
{
    return Super::GetSectionName();
}

FText USpawnerEditorSettings::GetSectionText() const
{
    return LOCTEXT("SpawerEditorSettings::GetSectionText", "Spawner");
}

FText USpawnerEditorSettings::GetSectionDescription() const
{
    return LOCTEXT("SpawerEditorSettings::GetSectionDescription", "Spawner Editor Settings");
}
