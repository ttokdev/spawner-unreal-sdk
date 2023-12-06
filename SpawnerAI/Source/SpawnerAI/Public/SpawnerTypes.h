// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnerTypes.generated.h"

USTRUCT(BlueprintType)
struct FSpawnerWorkspaceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Workspace")
	FString WorkspaceId;

	UPROPERTY(EditAnywhere, Category = "Workspace")
	FString ApiKey;
};

USTRUCT(BlueprintType)
struct FSpawnerAuthRequestData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "WebSocketRequest")
	FString apiKey;

	UPROPERTY(EditAnywhere, Category = "WebSocketRequest")
	FString chatId;

	UPROPERTY(EditAnywhere, Category = "WebSocketRequest")
	FString workspaceId;
};

USTRUCT(BlueprintType)
struct FSpawnerAuthRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "WebSocketRequest")
	FString type;

	UPROPERTY(EditAnywhere, Category = "WebSocketRequest")
	FSpawnerAuthRequestData data;
};

USTRUCT(BlueprintType)
struct FSpawnerAuthResponseData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "WebSocketResponse")
	bool auth;

	UPROPERTY(EditAnywhere, Category = "WebSocketResponse")
	FString message;

	UPROPERTY(EditAnywhere, Category = "WebSocketResponse")
	FString chat_id;
};

USTRUCT(BlueprintType)
struct FSpawnerAuthRespose
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "WebSocketResponse")
	FString type;

	UPROPERTY(EditAnywhere, Category = "WebSocketResponse")
	FSpawnerAuthResponseData data;
};

/* Send Chat */
USTRUCT(BlueprintType)
struct FSpawnerChatRequestData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "WebSocketRequest")
	FString chatId;

	UPROPERTY(EditAnywhere, Category = "WebSocketRequest")
	FString characterId;

	UPROPERTY(EditAnywhere, Category = "WebSocketRequest")
	FString input;

	UPROPERTY(EditAnywhere, Category = "WebSocketRequest")
	FString workspaceId;
};

USTRUCT(BlueprintType)
struct FSpawnerChatRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "WebSocketRequest")
	FString type;

	UPROPERTY(EditAnywhere, Category = "WebSocketRequest")
	FSpawnerChatRequestData data;
};

USTRUCT(BlueprintType)
struct FSpawnerResponseStreamData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "WebSocketResponse")
	FString chat_id;

	UPROPERTY(EditAnywhere, Category = "WebSocketResponse")
	FString character_id;

	UPROPERTY(EditAnywhere, Category = "WebSocketResponse")
	FString input;

	UPROPERTY(EditAnywhere, Category = "WebSocketResponse")
	FString response;

	UPROPERTY(EditAnywhere, Category = "WebSocketResponse")
	bool is_end;

	UPROPERTY(EditAnywhere, Category = "WebSocketResponse")
	bool blocked;
};

USTRUCT(BlueprintType)
struct FSpawnerResponseStream
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "WebSocketResponse")
	FString type;

	UPROPERTY(EditAnywhere, Category = "WebSocketResponse")
	FSpawnerResponseStreamData data;
};