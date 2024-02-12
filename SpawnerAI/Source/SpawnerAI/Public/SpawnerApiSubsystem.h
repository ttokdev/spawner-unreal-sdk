// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Http.h"
#include "SpawnerTypes.h"
#include "SpawnerApiSubsystem.generated.h"

class IWebSocket;
class USpawnerCharacterComponent;

UCLASS()
class SPAWNERAI_API USpawnerApiSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void StartSession(const FString& ApiKey, const FString& ApiSecret, const FString& WorkspaceId);

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void SendChat(const FString& CharacterId, const FString& InputText);

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	const FSpawnerWorkspaceData& GetWorkspaceData();
	
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void SetWorkspaceData(const FSpawnerWorkspaceData& Data);

	FORCEINLINE FString GetSessionToken() { return SessionToken; } const
	FORCEINLINE void SetSessionToken(const FString& token) { SessionToken = token; }
	FORCEINLINE FString GetRefreshToken() { return RefreshToken; } const
	FORCEINLINE void SetRefreshToken(const FString& token) { RefreshToken = token; }

	/** Subsystem interface */
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** Scene Characters */
	FORCEINLINE TArray<FString> GetSceneCharactersList() const { return SceneCharactersList; }
	void RegisterSceneCharacter(USpawnerCharacterComponent* CharacterComponent);

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	FORCEINLINE FString GetTargetCharacter() const { return TargetCharacterId; }

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	FORCEINLINE void SetTargetCharacter(FString target) { TargetCharacterId = target; }

	/** Player **/
	FORCEINLINE FSpawnerPlayerInfo GetCurrentPlayer() const { return PlayerInfo; }
	void RegisterPlayerInfo(FSpawnerPlayerInfo Player);
private:

	/** HTTP */
	void RequestSessionToken(const FString& ApiKey, const FString& ApiSecret, const FString& WorkspaceId);
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSucessfully);

	FString SessionToken = "";
	FString RefreshToken = "";
	int ExpiresTime = 0;

	/** WebSockets */
	TSharedPtr<IWebSocket> WebSocket = nullptr;

	void OnSocketConnected() const;
	void OnSocketConnectionError(const FString& err) const;
	void OnSocketClosed(const int statusCode, const FString& reason, const bool bWasClean) const;
	void OnSocketMessageReceived(const FString& message);

	/** Scene Characters */
	TArray<USpawnerCharacterComponent*> RegisteredCharacterComponents;
	TMap<FString, USpawnerCharacterComponent*> CharacterComponentsById;
	TArray<FString> SceneCharactersList;
	FString TargetCharacterId;

	/** Player **/
	FSpawnerPlayerInfo PlayerInfo;

	/* Utils */
	TSharedPtr<FJsonObject> ConvertJsonStringToObject(const FString& JsonString);
};
