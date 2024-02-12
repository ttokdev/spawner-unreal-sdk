// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnerTypes.generated.h"

USTRUCT(BlueprintType)
struct FSpawnerWorkspaceData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Workspace")
	FString WorkspaceId;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Workspace")
	FString ApiKey;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Workspace")
	FString ApiSecret;
};

USTRUCT(BlueprintType)
struct FSpawnerSessionTokenRequestData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Http Request")
	FString api_key;

	UPROPERTY(EditAnywhere, Category = "Http Request")
	FString api_secret;

	UPROPERTY(EditAnywhere, Category = "Http Request")
	FString workspace_id;

	UPROPERTY(EditAnywhere, Category = "Http Request")
	FString source = "UNREAL_ENGINE";
};

USTRUCT(BlueprintType)
struct FSpawnerSessionTokenResponseData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Http Response")
	FString session_token;

	UPROPERTY(EditAnywhere, Category = "Http Response")
	FString refresh_token;

	UPROPERTY(EditAnywhere, Category = "Http Response")
	FString token_type;

	UPROPERTY(EditAnywhere, Category = "Http Response")
	int expires_in;
};

USTRUCT(BlueprintType)
struct FSpawnerPlayerData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "WebSockets Request")
	FString id;

	UPROPERTY(EditAnywhere, Category = "WebSockets Request")
	FString name;

	UPROPERTY(EditAnywhere, Category = "WebSockets Request")
	FString description;

	UPROPERTY(EditAnywhere, Category = "WebSockets Request")
	FString age_group;

	UPROPERTY(EditAnywhere, Category = "WebSockets Request")
	FString gender;
};

USTRUCT(BlueprintType)
struct FSpawnerChatRequestData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "WebSockets Request")
	FString session_token;

	UPROPERTY(EditAnywhere, Category = "WebSockets Request")
	TArray<FString> characters;

	UPROPERTY(EditAnywhere, Category = "WebSockets Request")
	FString speaker_id;

	UPROPERTY(EditAnywhere, Category = "WebSockets Request")
	FString input_message;

	UPROPERTY(EditAnywhere, Category = "WebSockets Request")
	FSpawnerPlayerData player;
};

USTRUCT(BlueprintType)
struct FSpawnerResponseStreamData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "WebSockets Response")
	FString character_id;

	UPROPERTY(EditAnywhere, Category = "WebSockets Response")
	FString player_id;

	UPROPERTY(EditAnywhere, Category = "WebSockets Response")
	FString response;

	UPROPERTY(EditAnywhere, Category = "WebSockets Response")
	bool is_end;

	UPROPERTY(EditAnywhere, Category = "WebSockets Response")
	bool blocked;
};

USTRUCT(BlueprintType)
struct FSpawnerResponseStream
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "WebSockets Response")
	FString type;

	UPROPERTY(EditAnywhere, Category = "WebSockets Response")
	FSpawnerResponseStreamData data;
};

UENUM(BlueprintType)
enum class EPlayerAgeGroup : uint8
{
	EP_Child UMETA(DisplayName = "CHILD"),
	EP_Teen UMETA(DisplayName = "TEEN"),
	EP_Adult UMETA(DisplayName = "ADULT"),
	EP_Elderly UMETA(DisplayName = "ELDERLY"),
};

UENUM(BlueprintType)
enum class EPlayerGender : uint8
{
	EP_Child UMETA(DisplayName = "MALE"),
	EP_Teen UMETA(DisplayName = "FEMALE"),
};

/** For Unreal **/
USTRUCT(BlueprintType)
struct FSpawnerPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "WebSockets Request")
	FString Id;

	UPROPERTY(EditAnywhere, Category = "WebSockets Request")
	FString Name;

	UPROPERTY(EditAnywhere, Category = "WebSockets Request")
	FString Description;

	UPROPERTY(EditAnywhere, Category = "WebSockets Request")
	EPlayerAgeGroup AgeGroup;

	UPROPERTY(EditAnywhere, Category = "WebSockets Request")
	EPlayerGender Gender;
};