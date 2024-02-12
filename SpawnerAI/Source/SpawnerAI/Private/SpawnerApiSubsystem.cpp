// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerApiSubsystem.h"
#include "WebSocketsModule.h"
#include "IWebSocket.h"
#include "Http.h"
#include "JsonObjectConverter.h"
#include "SpawnerAIModule.h"
#include "SpawnerCharacterComponent.h"

void USpawnerApiSubsystem::StartSession(const FString& ApiKey, const FString& ApiSecret, const FString& WorkspaceId)
{
	if (ApiKey.IsEmpty())
	{
		UE_LOG(LogSpawnerAI, Error, TEXT("Can't start session, ApiKey is empty"));
		return;
	}
	if (ApiSecret.IsEmpty())
	{
		UE_LOG(LogSpawnerAI, Error, TEXT("Can't start session, ApiSecret is empty"));
		return;
	}
	if (WorkspaceId.IsEmpty())
	{
		UE_LOG(LogSpawnerAI, Error, TEXT("Can't start session, WorkspaceId is empty"));
		return;
	}

	UE_LOG(LogSpawnerAI, Log, TEXT("Try to start session."));

	RequestSessionToken(ApiKey, ApiSecret, WorkspaceId);
	WebSocket->Connect();
}

void USpawnerApiSubsystem::SendChat(const FString& CharacterId, const FString& InputText)
{
	if (InputText.IsEmpty())
	{
		UE_LOG(LogSpawnerAI, Error, TEXT("Input text cannot be empty."));
		return;
	}

	if (!WebSocket->IsConnected())
	{
		UE_LOG(LogSpawnerAI, Error, TEXT("Websocket must be connected to send chat."));
		return;
	}
	
	if (SessionToken.IsEmpty())
	{
		UE_LOG(LogSpawnerAI, Error, TEXT("The session token is required to send chat."));
		return;
	}
	FSpawnerChatRequestData ChatRequestData;
	ChatRequestData.session_token = SessionToken;
	ChatRequestData.characters = SceneCharactersList;
	ChatRequestData.speaker_id = CharacterId;
	ChatRequestData.input_message = InputText;

	if (PlayerInfo.Id.IsEmpty())
	{
		UE_LOG(LogSpawnerAI, Error, TEXT("Player ID is required."));
		return;
	}
	UE_LOG(LogSpawnerAI, Log, TEXT("Player Id string: %s"), *PlayerInfo.Id);
	FSpawnerPlayerData Player;
	Player.id = PlayerInfo.Id;
	Player.name = PlayerInfo.Name;
	Player.description = PlayerInfo.Description;
	Player.age_group = PlayerInfo.AgeGroup;
	Player.gender = PlayerInfo.Gender;

	ChatRequestData.player = Player;
	
	FString ChatRequestBody;
	FJsonObjectConverter::UStructToJsonObjectString(ChatRequestData, ChatRequestBody);

	UE_LOG(LogSpawnerAI, Log, TEXT("request json string: %s"), *ChatRequestBody);

	WebSocket->Send(ChatRequestBody);
}

const FSpawnerWorkspaceData& USpawnerApiSubsystem::GetWorkspaceData()
{
	FSpawnerAIModule* Module = static_cast<FSpawnerAIModule*>(FModuleManager::Get().GetModule("SpawnerAI"));
	if (ensure(Module))
	{
		return Module->GetWorkspaceData();
	}
	else 
	{
		static FSpawnerWorkspaceData Data;
		return Data;
	}
}

void USpawnerApiSubsystem::SetWorkspaceData(const FSpawnerWorkspaceData& Data)
{
	FSpawnerAIModule* Module = static_cast<FSpawnerAIModule*>(FModuleManager::Get().GetModule("SpawnerAI"));
	if (ensure(Module))
	{
		return Module->SetWorkspaceData(Data);
	}
}

bool USpawnerApiSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!Super::ShouldCreateSubsystem(Outer))
	{
		return false;
	}

	FSpawnerAIModule* Module = static_cast<FSpawnerAIModule*>(FModuleManager::Get().GetModule("SpawnerAI"));
	if (ensure(Module))
	{
		if (Module->ShouldInitializeApi())
		{
			return true;
		}
		else {
			return false;
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
	UE_LOG(LogSpawnerAI, Log, TEXT("SpawnerApiSubsystem was initialized."));

	/** WebSocket initialize **/
	if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
	{
		FModuleManager::Get().LoadModule("WebSockets");
	}

	const FString API_URI = TEXT("ws://localhost:8000/socket");
	const FString Protocol = TEXT("ws");

	WebSocket = FWebSocketsModule::Get().CreateWebSocket(API_URI, Protocol);

	WebSocket->OnConnected().AddUObject(this, &USpawnerApiSubsystem::OnSocketConnected);
	WebSocket->OnConnectionError().AddUObject(this, &USpawnerApiSubsystem::OnSocketConnectionError);
	WebSocket->OnClosed().AddUObject(this, &USpawnerApiSubsystem::OnSocketClosed);
	WebSocket->OnMessage().AddUObject(this, &USpawnerApiSubsystem::OnSocketMessageReceived);
}

void USpawnerApiSubsystem::Deinitialize()
{
	if (WebSocket->IsConnected())
	{
		WebSocket->Close();
		WebSocket = nullptr;
	}
	UE_LOG(LogSpawnerAI, Log, TEXT("SpawnerApiSubsystem was deinitialized."));
}

void USpawnerApiSubsystem::RegisterSceneCharacter(USpawnerCharacterComponent* CharacterComponent)
{
	FString CharacterId = CharacterComponent->GetCharacterId();
	if (!ensure(!CharacterComponentsById.Contains(CharacterId)))
	{
		UE_LOG(LogSpawnerAI, Error, TEXT("Component already registered for Character Id: %s"), *CharacterId);
		return;
	}
	SceneCharactersList.Add(CharacterId);
	RegisteredCharacterComponents.Add(CharacterComponent);
	CharacterComponentsById.Add(CharacterId, CharacterComponent);

	if ((SceneCharactersList.Num() == 1))
	{
		SetTargetCharacter(SceneCharactersList[0]);
	}
}

void USpawnerApiSubsystem::RegisterPlayerInfo(FSpawnerPlayerInfo Player)
{
	PlayerInfo = Player;
}

void USpawnerApiSubsystem::RequestSessionToken(const FString& ApiKey, const FString& ApiSecret, const FString& WorkspaceId)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	FSpawnerSessionTokenRequestData RequestData;
	
	RequestData.api_key = ApiKey;
	RequestData.api_secret = ApiSecret;
	RequestData.workspace_id = WorkspaceId;

	FString SessionTokenRequestBody;
	FJsonObjectConverter::UStructToJsonObjectString(RequestData, SessionTokenRequestBody);
	
	Request->OnProcessRequestComplete().BindUObject(this, &USpawnerApiSubsystem::OnResponseReceived);
	Request->SetURL("localhost:8000/token");
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(SessionTokenRequestBody);

	Request->ProcessRequest();
}

void USpawnerApiSubsystem::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSucessfully)
{
	FSpawnerSessionTokenResponseData SessionTokenResponse;
	FJsonObjectConverter::JsonObjectStringToUStruct<FSpawnerSessionTokenResponseData>(Response->GetContentAsString(), &SessionTokenResponse, 0,0);

	if (bConnectedSucessfully)
	{
		SetSessionToken(SessionTokenResponse.session_token);
		SetRefreshToken(SessionTokenResponse.refresh_token);

		return;
	}
	else 
	{
		UE_LOG(LogSpawnerAI, Error, TEXT("Http connection failed."));
		return;
	}
}

void USpawnerApiSubsystem::OnSocketConnected() const
{
	UE_LOG(LogSpawnerAI, Log, TEXT("WebSocket Connected Successful"));
}

void USpawnerApiSubsystem::OnSocketConnectionError(const FString& err) const
{
	UE_LOG(LogSpawnerAI, Error, TEXT("WebSocket Connected Error: %s"), *err);
}

void USpawnerApiSubsystem::OnSocketClosed(const int statusCode, const FString& reason, const bool bWasClean) const
{
	UE_LOG(LogSpawnerAI, Log, TEXT("WebSocket Closed: %s"), *reason);
}

void USpawnerApiSubsystem::OnSocketMessageReceived(const FString& message)
{
	UE_LOG(LogSpawnerAI, Log, TEXT("WebSokcet Received: %s"), *message);

	TSharedPtr<FJsonObject> PayloadObject = ConvertJsonStringToObject(message);
	FString ResponseType = "";

	if (PayloadObject != nullptr) {
		ResponseType = PayloadObject->GetStringField("type");
	}
	
	if (ResponseType == "RESPONSE_STREAM")
	{
		UE_LOG(LogSpawnerAI, Log, TEXT("Response type is RESPONSE_STREAM."));
		FSpawnerResponseStream ResponseStream;
		FJsonObjectConverter::JsonObjectStringToUStruct<FSpawnerResponseStream>(message, &ResponseStream, 0, 0);

		if(CharacterComponentsById.Contains(TargetCharacterId))
		{
			USpawnerCharacterComponent* Component = CharacterComponentsById[TargetCharacterId];
			Component->OnCharacterTextStreamReceived.Broadcast(ResponseStream.data.response, ResponseStream.data.is_end);
		}
		
		return;
	}
	else if (ResponseType == "EMOTION")
	{
		UE_LOG(LogSpawnerAI, Log, TEXT("Response type is EMOTION."));
		return;
	}
	else
	{
		UE_LOG(LogSpawnerAI, Log, TEXT("Response type is not valid."));
		return;
	}
	
}

TSharedPtr<FJsonObject> USpawnerApiSubsystem::ConvertJsonStringToObject(const FString& JsonString)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		return JsonObject;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to convert JsonString to JsonObject."));
		return nullptr;
	}
}