#include "SpawnerEditorModule.h"
#include "Engine/SCS_Node.h"
#include "ContentBrowserModule.h"
#include "EditorUtilityWidget.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "EditorUtilitySubsystem.h"
#include "SpawnerEditorSettings.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Interfaces/IMainFrameModule.h"

#define LOCTEXT_NAMESPACE "FSpawnerEditorModule"

void FSpawnerEditorModule::StartupModule()
{
	IMainFrameModule& MainFrameModule = IMainFrameModule::Get();
	RegisterSpawnerEditorWidgetDelegate = MainFrameModule.OnMainFrameCreationFinished().AddRaw(this, &FSpawnerEditorModule::RegisterSpawnerEditorStudioWidget);

	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
	TArray<FContentBrowserMenuExtender_SelectedAssets>& ContentBrowserMenuExtenderDelegates = ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
	ContentBrowserMenuExtenderDelegates.Add(FContentBrowserMenuExtender_SelectedAssets::CreateRaw(this, &FSpawnerEditorModule::OnExtendAssetSelectionMenu));
}

void FSpawnerEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	IMainFrameModule& MainFrameModule = IMainFrameModule::Get();
	MainFrameModule.OnMainFrameCreationFinished().Remove(RegisterSpawnerEditorWidgetDelegate);
}

void FSpawnerEditorModule::RegisterSpawnerEditorStudioWidget(TSharedPtr<SWindow>, bool)
{
	FSoftObjectPath SpawnerEditorWidgetPath(TEXT("/SpawnerAI/SpawnerEditorWidget.SpawnerEditorWidget"));
	TSoftObjectPtr<UObject> Asset(SpawnerEditorWidgetPath);

	if (UWidgetBlueprint* Blueprint = Cast<UWidgetBlueprint>(Asset.LoadSynchronous()))
	{
		if (Blueprint->GeneratedClass->IsChildOf(UEditorUtilityWidget::StaticClass()))
		{
			UEditorUtilityWidgetBlueprint* EditorWidget = Cast<UEditorUtilityWidgetBlueprint>(Blueprint);
			if (EditorWidget)
			{
				UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
				FName ID;
				EditorUtilitySubsystem->RegisterTabAndGetID(EditorWidget, ID);
			}
		}
	}
}

TSharedRef<FExtender> FSpawnerEditorModule::OnExtendAssetSelectionMenu(const TArray<FAssetData>& SelectedAssets)
{
	TSharedRef<FExtender> Extender = MakeShared<FExtender>();
	
	Extender->AddMenuExtension(
		"CommonAssetActions",
		EExtensionHook::After,
		nullptr,
		FMenuExtensionDelegate::CreateRaw(this, &FSpawnerEditorModule::OnAssetExtension, SelectedAssets)
	);

	return Extender;
}

void FSpawnerEditorModule::OnAssetExtension(FMenuBuilder& MenuBuilder, const TArray<FAssetData> SelectedAssets)
{
	for (auto& Asset : SelectedAssets)
	{
		UBlueprint* const Blueprint = Cast<UBlueprint>(Asset.GetAsset());
		if (!Blueprint || !Blueprint->SimpleConstructionScript)
		{
			return;
		}
	}

	MenuBuilder.BeginSection("Spawner", FText::FromString("Spawner"));

	MenuBuilder.AddSubMenu(
		FText::FromString("Spawner Actions"),
		FText::FromString("Setup Spawner Blueprint Components"),
		FNewMenuDelegate::CreateRaw(this, &FSpawnerEditorModule::OnSubMenuExtention, SelectedAssets)
	);

	MenuBuilder.EndSection();
}

void FSpawnerEditorModule::OnSubMenuExtention(FMenuBuilder& SubMenuBuider, const TArray<FAssetData> SelectedAssets)
{
	SubMenuBuider.BeginSection("Player", FText::FromString("Player"));

	SubMenuBuider.AddMenuEntry(
		FText::FromString("Setup as Spawner Player"),
		FText::FromString("Add Components as Spawner Player to this Actor"),
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateRaw(this, &FSpawnerEditorModule::SetupAsSpawnerPlayer, SelectedAssets)
		)
	);

	SubMenuBuider.EndSection();

	SubMenuBuider.BeginSection("Character", FText::FromString("Character"));

	SubMenuBuider.AddMenuEntry(
		FText::FromString("Setup as Spawner Character"),
		FText::FromString("Add Components as Spawner Character to this Actor"),
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateRaw(this, &FSpawnerEditorModule::SetupAsSpawnerCharacter, SelectedAssets)
		)
	);

	SubMenuBuider.EndSection();
}

void FSpawnerEditorModule::SetupAsSpawnerPlayer(const TArray<FAssetData> SelectedAssets)
{
	if (SelectedAssets.Num() > 0)
	{
		for (const auto& SelectedAsset : SelectedAssets)
		{
			auto* Object = SelectedAsset.GetAsset();
			auto* Blueprint = Cast<UBlueprint>(Object);
			
			const USpawnerEditorSettings* SpawnerEditorSettings = GetDefault<USpawnerEditorSettings>();

			const TSubclassOf<USpawnerPlayerComponent> SpawnerPlayerComponent = SpawnerEditorSettings->SpawnerPlayerComponent;
		
			USCS_Node* BPNode = Blueprint->SimpleConstructionScript->CreateNode(SpawnerPlayerComponent, *SpawnerPlayerComponent->GetName());
			Blueprint->SimpleConstructionScript->AddNode(BPNode);
			FKismetEditorUtilities::CompileBlueprint(Blueprint);
		}
	}
}

void FSpawnerEditorModule::SetupAsSpawnerCharacter(const TArray<FAssetData> SelectedAssets)
{
	if (SelectedAssets.Num() > 0)
	{
		for (const auto& SelectedAsset : SelectedAssets)
		{
			auto* Object = SelectedAsset.GetAsset();
			auto* Blueprint = Cast<UBlueprint>(Object);

			const USpawnerEditorSettings* SpawnerEditorSettings = GetDefault<USpawnerEditorSettings>();

			const TSubclassOf<USpawnerCharacterComponent> SpawnerCharacterComponent = SpawnerEditorSettings->SpawnerCharacterComponent;

			USCS_Node* BPNode = Blueprint->SimpleConstructionScript->CreateNode(SpawnerCharacterComponent, *SpawnerCharacterComponent->GetName());
			Blueprint->SimpleConstructionScript->AddNode(BPNode);
			FKismetEditorUtilities::CompileBlueprint(Blueprint);
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSpawnerEditorModule, SpawnerEditor)