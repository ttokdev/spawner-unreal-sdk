// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerApiSubsystem.h"
#include "SpawnerWorldSettings.h"
#include "WebSocketsModule.h"
#include "IWebSocket.h"
#include "JsonObjectConverter.h"
#include "SpawnerTypes.h"
#include "SpawnerAI.h"

void USpawnerApiSubsystem::StartSession(const FString& ApiKey, const FString& WorkspaceId, FString ChatId) const
{
	if (!WebSocket->IsConnected())
	{
		UE_LOG(LogTemp, Error, TEXT("Websocket must be connected to start session."));
		return;
	}
	if (ApiKey.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Can't Start Session, ApiKey is empty"));
		return;
	}
	if (WorkspaceId.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Can't Start Session, WorkspaceId is empty"));
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("StartSession"));

	if (ChatId.IsEmpty())
	{
		ChatId = GenerateRandomString(15);
	}

	FSpawnerAuthRequestData AuthRequestData;
	AuthRequestData.apiKey = ApiKey;
	AuthRequestData.chatId = ChatId;
	AuthRequestData.workspaceId = WorkspaceId;

	FSpawnerAuthRequest AuthRequest;
	AuthRequest.type = "AUTH";
	AuthRequest.data = AuthRequestData;
	
	FString OutAuthRequestJsonString;
	FJsonObjectConverter::UStructToJsonObjectString(AuthRequest, OutAuthRequestJsonString);

	WebSocket->Send(OutAuthRequestJsonString);
}

void USpawnerApiSubsystem::SendChat(const FString& CharacterId, const FString& InputText) const
{
	if (InputText.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Input text cannot be empty."));
	}
	FSpawnerWorkspaceData SpawnerWorkspaceData;
	FSpawnerAIModule* Module = static_cast<FSpawnerAIModule*>(FModuleManager::Get().GetModule("SpawnerAI"));
	if (ensure(Module))
	{
		SpawnerWorkspaceData = Module->GetWorkspaceData();
	}
	
	FSpawnerChatRequestData ChatRequestData;
	ChatRequestData.characterId = CharacterId;
	ChatRequestData.chatId = GetCurrentChatId();
	ChatRequestData.workspaceId = SpawnerWorkspaceData.WorkspaceId;
	ChatRequestData.input = InputText;

	FSpawnerChatRequest ChatRequest;
	ChatRequest.type = "SEND_CHAT";
	ChatRequest.data = ChatRequestData;
	
	FString OutChatRequestJsonString;
	FJsonObjectConverter::UStructToJsonObjectString(ChatRequest, OutChatRequestJsonString);

	WebSocket->Send(OutChatRequestJsonString);
}

bool USpawnerApiSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!Super::ShouldCreateSubsystem(Outer))
	{
		return false;
	}

	if (UWorld* WorldOuter = Cast<UWorld>(Outer))
	{
		if (ASpawnerWorldSettings* SpawnerWorldSettings = Cast<ASpawnerWorldSettings>(WorldOuter->GetWorldSettings()))
		{
			return SpawnerWorldSettings->bEnableSpawnerApi;
		}
	}

	return false;
}

bool USpawnerApiSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void USpawnerApiSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("SpawnerApiSubsystem was Initialized."));

	if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
	{
		FModuleManager::Get().LoadModule("WebSockets");
	}

	const FString API_URI = TEXT("wss://qnwztoc28f.execute-api.ap-northeast-1.amazonaws.com/dev");
	const FString Protocol = TEXT("wss");

	WebSocket = FWebSocketsModule::Get().CreateWebSocket(API_URI, Protocol);

	WebSocket->OnConnected().AddUObject(this, &USpawnerApiSubsystem::OnSocketConnected);
	WebSocket->OnConnectionError().AddUObject(this, &USpawnerApiSubsystem::OnSocketConnectionError);
	WebSocket->OnClosed().AddUObject(this, &USpawnerApiSubsystem::OnSocketClosed);
	WebSocket->OnMessage().AddUObject(this, &USpawnerApiSubsystem::OnSocketMessageReceived);

	WebSocket->Connect();
}

void USpawnerApiSubsystem::Deinitialize()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("USpawnerApiSubsystem::Deinitialize!!!"));
	if (WebSocket->IsConnected())
	{
		WebSocket->Close();
		WebSocket = nullptr;
	}
	UE_LOG(LogTemp, Log, TEXT("SpawnerApiSubsystem was Deinitialized."));
}

void USpawnerApiSubsystem::OnSocketConnected() const
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("WebSocket Connected Successful"));
	
	FSpawnerWorkspaceData SpawnerWorkspaceData;

	FSpawnerAIModule* Module = static_cast<FSpawnerAIModule*>(FModuleManager::Get().GetModule("SpawnerAI"));
	if (ensure(Module))
	{
		SpawnerWorkspaceData = Module->GetWorkspaceData();
	}

	StartSession(SpawnerWorkspaceData.ApiKey, SpawnerWorkspaceData.WorkspaceId);
}

void USpawnerApiSubsystem::OnSocketConnectionError(const FString& err) const
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("WebSocket Connected Error: " + err));
}

void USpawnerApiSubsystem::OnSocketClosed(const int statusCode, const FString& reason, const bool bWasClean) const
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, bWasClean ? FColor::Green : FColor::Red, TEXT("WebSocket Closed: " + reason));
	UE_LOG(LogTemp, Log, TEXT("WebSocket Closed: %s"), &reason);
}

void USpawnerApiSubsystem::OnSocketMessageReceived(const FString& message)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("ReceivedMessage: ") + message);

	TSharedPtr<FJsonObject> PayloadObject = ConvertJsonStringToObject(message);

	FString ResponseType = PayloadObject->GetStringField("type");

	if (ResponseType == "AUTH_RESPONSE")
	{
		UE_LOG(LogTemp, Log, TEXT("AUTH_RESPONSE type response received."));
		FSpawnerAuthRespose AuthResponse;
		FJsonObjectConverter::JsonObjectStringToUStruct<FSpawnerAuthRespose>(message, &AuthResponse, 0, 0);

		if (!AuthResponse.data.auth)
		{
			UE_LOG(LogTemp, Error, TEXT("Authentication is failed."));
			return;
		}

		SetIsAuthenticated(AuthResponse.data.auth);
		SetCurrentChatId(AuthResponse.data.chat_id);
		return;
	}
	else if (ResponseType == "RESPONSE_STREAM")
	{
		FSpawnerResponseStream ResponseStream;
		FJsonObjectConverter::JsonObjectStringToUStruct<FSpawnerResponseStream>(message, &ResponseStream, 0, 0);
		
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Response text: ") + ResponseStream.data.response);
		UE_LOG(LogTemp, Log, TEXT("Response text: %s"), *ResponseStream.data.response);
		return;
	}
	else if (ResponseType == "OA_MODERATION")
	{
		UE_LOG(LogTemp, Log, TEXT("OA_MODERATION type response received."));
		return;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Response type is not valid."));
		return;
	}
	
}

FString USpawnerApiSubsystem::GenerateRandomString(int32 Length) const
{
	const FString Characters = TEXT("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

	FString RandomString;
	RandomString.Reserve(Length);

	for (int32 i = 0; i < Length; ++i)
	{
		int32 RandomIndex = FMath::RandRange(0, Characters.Len() - 1);
		RandomString.AppendChar(Characters[RandomIndex]);
	}

	return RandomString;
}

TSharedPtr<FJsonObject> USpawnerApiSubsystem::ConvertJsonStringToObject(const FString& JsonString)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		return JsonObject;
	}
	return nullptr;
}
