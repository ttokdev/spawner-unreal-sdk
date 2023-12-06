// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTalkingActorComponent.h"
#include "WebSocketsModule.h"
#include "IWebSocket.h"

UMyTalkingActorComponent::UMyTalkingActorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMyTalkingActorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMyTalkingActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMyTalkingActorComponent::StartSession_2()
{
	if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
	{
		FModuleManager::Get().LoadModule("WebSockets");
	}

	const FString API_URI = TEXT("wss://qnwztoc28f.execute-api.ap-northeast-1.amazonaws.com/dev");
	const FString Protocol = TEXT("wss");

	WebSocket = FWebSocketsModule::Get().CreateWebSocket(API_URI, Protocol);

	WebSocket->OnConnected().AddUObject(this, &UMyTalkingActorComponent::OnSocketConnected);
	WebSocket->OnConnectionError().AddUObject(this, &UMyTalkingActorComponent::OnSocketConnectionError);
	WebSocket->OnClosed().AddUObject(this, &UMyTalkingActorComponent::OnSocketClosed);
	WebSocket->OnMessage().AddUObject(this, &UMyTalkingActorComponent::OnSocketMessageReceived);

	WebSocket->Connect();
}

void UMyTalkingActorComponent::CloseSession()
{
	if (WebSocket->IsConnected())
	{
		WebSocket->Close();
		WebSocket = nullptr;
	}
}

void UMyTalkingActorComponent::OnSocketConnected() const
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("WebSocket Connected Successful"));
	
}

void UMyTalkingActorComponent::OnSocketConnectionError(const FString& err) const
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("WebSocket Connected Error: " + err));
}

void UMyTalkingActorComponent::OnSocketClosed(const int statusCode, const FString& reason, const bool bWasClean) const
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, bWasClean ? FColor::Green : FColor::Red, TEXT("WebSocket Closed: " + reason));;
}

void UMyTalkingActorComponent::OnSocketMessageReceived(const FString& message)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("ReceivedMessage: ") + message);

	// Jsonオブジェクト変数の初期化
	TSharedPtr<FJsonObject> PayloadObj = MakeShareable(new FJsonObject());
	// 文字列からJsonを読み込むためのReader初期化
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(*message);

	if (FJsonSerializer::Deserialize(Reader, PayloadObj))
	{
		// 受け取ったレスポンスの内容をすべて表示する
		UE_LOG(LogTemp, Log, TEXT("Response: %s"), *message);

		// レスポンスのタイプを格納する
		FString ResponseType = PayloadObj->GetStringField("type");

		// レスポンスのタイプが"AUTH_RESPONSE"の場合
		if (ResponseType == "AUTH_RESPONSE") {
			UE_LOG(LogTemp, Log, TEXT("Response Type: %s"), *ResponseType);

			TSharedPtr<FJsonObject> DataObj = PayloadObj->GetObjectField("data");

			if (DataObj->HasField("chatId")) {
				FString AuthenticatedChatId = DataObj->GetStringField("chatId");
				SetChatId(AuthenticatedChatId);
			}
		}

		// レスポンスのタイプが"RESPONSE_CHAT"の場合
		else if (ResponseType == "RESPONSE_CHAT") {

			UE_LOG(LogTemp, Log, TEXT("Response Type: %s"), *ResponseType);

			// レスポンス内部のデータオブジェクトを取得する
			TSharedPtr<FJsonObject> DataObj = PayloadObj->GetObjectField("data");

			if (!DataObj->HasField("response")) return;

			FString ChatResponse = DataObj->GetStringField("response");
			// 返答されたチャットの中身をログに表示する
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Chat Response: ") + ChatResponse);
			UE_LOG(LogTemp, Log, TEXT("Chat Response: %s"), *ChatResponse);
		}

		else {
			UE_LOG(LogTemp, Log, TEXT("Response Type: %s"), *ResponseType);
		}
	}
}

void UMyTalkingActorComponent::Authenticate()
{
	TSharedPtr<FJsonObject> PayloadObj = MakeShareable(new FJsonObject());
	TSharedPtr<FJsonObject> DataObj = MakeShareable(new FJsonObject());

	FString chatId = GenerateRandomString(15);

	DataObj->SetStringField("apiKey", "sk-IybAAx28ARicGzryMAwa1hx4N5zSGFAuqm97JQeYrjZbfl4k");
	DataObj->SetStringField("chatId", chatId);
	DataObj->SetStringField("workspaceId", "co3e36dEHTasqrO");

	PayloadObj->SetStringField("type", "AUTH");
	PayloadObj->SetObjectField("data", DataObj);

	FString SendAuthJsonString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&SendAuthJsonString);
	FJsonSerializer::Serialize(PayloadObj.ToSharedRef(), JsonWriter);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, OutputString);

	WebSocket->Send(SendAuthJsonString);
}

void UMyTalkingActorComponent::SendChat(const FString InputMessage)
{
	if (GetChatId().IsEmpty()) {
		UE_LOG(LogTemp, Error, TEXT("Chat id is not valid."));
		return;
	}

	FString CurrentChatId = GetChatId();

	TSharedPtr<FJsonObject> PayloadObj = MakeShareable(new FJsonObject());
	TSharedPtr<FJsonObject> DataObj = MakeShareable(new FJsonObject());

	DataObj->SetStringField("chatId", CurrentChatId);
	DataObj->SetStringField("characterId", "AfdDVnZGUgsIWVi");
	DataObj->SetStringField("workspaceId", "co3e36dEHTasqrO");
	DataObj->SetStringField("input", InputMessage);

	PayloadObj->SetStringField("type", "SEND_CHAT");
	PayloadObj->SetObjectField("data", DataObj);

	FString SendChatJsonString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&SendChatJsonString);
	FJsonSerializer::Serialize(PayloadObj.ToSharedRef(), JsonWriter);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, OutputString);
	WebSocket->Send(SendChatJsonString);
}

FString UMyTalkingActorComponent::GenerateRandomString(int32 Length)
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
