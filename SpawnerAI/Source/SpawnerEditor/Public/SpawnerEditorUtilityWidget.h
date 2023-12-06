// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "SpawnerEditorUtilityWidget.generated.h"

/**
 * 
 */

UCLASS(Abstract)
class SPAWNEREDITOR_API USpawnerEditorUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "User Interface")
	void ChangeWorkspaceIdText(const FText& Text);

	UFUNCTION(BlueprintCallable, Category = "User Interface")
	void ChangeApiKeyText(const FText& Text);

	UFUNCTION(BlueprintCallable, Category = "User Interface")
	void ChangeWorkspaceData(const FText& Text, const FString& FieldName);
};
