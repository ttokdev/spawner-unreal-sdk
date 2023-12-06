// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerEditorUtilityWidget.h"
#include "SpawnerAI.h"
#include "Components/EditableTextBox.h"
#include "SpawnerTypes.h"

void USpawnerEditorUtilityWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UEditableTextBox* WorkspaceIdEditableTextBox = Cast<UEditableTextBox>(GetWidgetFromName("WorkspaceIdTextBox"));
	if (WorkspaceIdEditableTextBox) {
		WorkspaceIdEditableTextBox->OnTextChanged.AddDynamic(this, &USpawnerEditorUtilityWidget::ChangeWorkspaceIdText);
	}

	UEditableTextBox* ApiKeyEditableTextBox = Cast<UEditableTextBox>(GetWidgetFromName("ApiKeyTextBox"));
	if (ApiKeyEditableTextBox) {
		ApiKeyEditableTextBox->OnTextChanged.AddDynamic(this, &USpawnerEditorUtilityWidget::ChangeApiKeyText);
	}
}

void USpawnerEditorUtilityWidget::ChangeWorkspaceIdText(const FText& Text)
{
	FString FieldName = TEXT("WorkspaceId");
	ChangeWorkspaceData(Text, FieldName);
}

void USpawnerEditorUtilityWidget::ChangeApiKeyText(const FText& Text)
{
	FString FieldName = TEXT("ApiKey");
	ChangeWorkspaceData(Text, FieldName);
}

void USpawnerEditorUtilityWidget::ChangeWorkspaceData(const FText& Text, const FString& FieldName)
{
	FSpawnerAIModule* Module = static_cast<FSpawnerAIModule*>(FModuleManager::Get().GetModule("SpawnerAI"));
	if (ensure(Module))
	{
		FSpawnerWorkspaceData Data = Module->GetWorkspaceData();
		if (FieldName == "WorkspaceId")
		{
			Data.WorkspaceId = Text.ToString();
		}
		if (FieldName == "ApiKey")
		{
			Data.ApiKey = Text.ToString();
		}
		Module->SetWorkspaceData(Data);
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, "Workspace id: " + Data.WorkspaceId + " Api Key: " + Data.ApiKey);
	}
}


