/** Copyright 2024 Warps, Inc. **/

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SpawnerEditorSettings.generated.h"

class USpawnerPlayerComponent;
class USpawnerCharacterComponent;

UCLASS(config=Spawner)
class SPAWNEREDITOR_API USpawnerEditorSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	USpawnerEditorSettings();

	virtual FName GetContainerName() const override;
	virtual FName GetCategoryName() const override;
	virtual FName GetSectionName() const override;

#if WITH_EDITOR
	virtual FText GetSectionText() const override;
	virtual FText GetSectionDescription() const override;
#endif
	void EnableApi(bool IsEnabled) { EnableSpawnerAPI = IsEnabled; };

	void OnEnableApiSettingChanged(UObject*, struct FPropertyChangedEvent&);
	UPROPERTY(config, EditAnywhere, Category = "API")
	bool EnableSpawnerAPI;

	UPROPERTY(config, EditAnywhere, Category = "Player")
	TSubclassOf<USpawnerPlayerComponent> SpawnerPlayerComponent;

	UPROPERTY(config, EditAnywhere, Category = "Character")
	TSubclassOf<USpawnerCharacterComponent> SpawnerCharacterComponent;
};
