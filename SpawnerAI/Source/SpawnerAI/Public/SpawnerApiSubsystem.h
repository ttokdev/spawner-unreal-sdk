// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SpawnerApiSubsystem.generated.h"

class IWebSocket;

UCLASS()
class SPAWNERAI_API USpawnerApiSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void StartSession(const FString& ApiKey, const FString& WorkspaceId, FString ChatId = "") const;

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void SendChat(const FString& CharacterId, const FString& InputText) const;

	FORCEINLINE bool CheckIsAuthenticated() { return bIsAuthenticated; }
	FORCEINLINE void SetIsAuthenticated(bool NewValue) { bIsAuthenticated = NewValue; }
	FORCEINLINE FString GetCurrentChatId() const { return CurrentChatId; }
	FORCEINLINE void SetCurrentChatId(const FString& ChatId) { CurrentChatId = ChatId; }

	/** Subsystem interface */
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
private:
	TSharedPtr<IWebSocket> WebSocket = nullptr;

	bool bIsAuthenticated;
	FString CurrentChatId = "";

	void OnSocketConnected() const;
	void OnSocketConnectionError(const FString& err) const;
	void OnSocketClosed(const int statusCode, const FString& reason, const bool bWasClean) const;
	void OnSocketMessageReceived(const FString& message);

	/* Utils */
	FString GenerateRandomString(int32 Length) const;
	TSharedPtr<FJsonObject> ConvertJsonStringToObject(const FString& JsonString);
};
