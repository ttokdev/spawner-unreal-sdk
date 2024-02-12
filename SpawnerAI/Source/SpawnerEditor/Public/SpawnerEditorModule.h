#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "SpawnerTypes.h"

class FSpawnerEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	FORCEINLINE const FSpawnerWorkspaceData& GetEditorWorkspaceData() { return EditorWorkspaceData; }
	FORCEINLINE void SetEditorWorkspaceData(const FSpawnerWorkspaceData& Data) { EditorWorkspaceData = Data; }

private:

	FSpawnerWorkspaceData EditorWorkspaceData;

	FDelegateHandle RegisterSpawnerEditorWidgetDelegate;
	void RegisterSpawnerEditorStudioWidget(TSharedPtr<SWindow>, bool);

	TSharedRef<FExtender> OnExtendAssetSelectionMenu(const TArray<FAssetData>& SelectedAssets);
	void OnAssetExtension(FMenuBuilder& MenuBuilder, const TArray<FAssetData> SelectedAssets);
	void OnSubMenuExtention(FMenuBuilder& SubMenuBuider, const TArray<FAssetData> SelectedAssets);
	void SetupAsSpawnerPlayer(const TArray<FAssetData> SelectedAssets);
	void SetupAsSpawnerCharacter(const TArray<FAssetData> SelectedAssets);
};