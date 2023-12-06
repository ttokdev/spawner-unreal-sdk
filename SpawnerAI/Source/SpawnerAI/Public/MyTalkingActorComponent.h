// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyTalkingActorComponent.generated.h"

class IWebSocket;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPAWNERAI_API UMyTalkingActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMyTalkingActorComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Spawner")
	void StartSession_2();

	UFUNCTION(BlueprintCallable)
	void CloseSession();

	UFUNCTION(BlueprintCallable)
	void Authenticate();

	UFUNCTION(BlueprintCallable)
	void SendChat(const FString InputMessage);
protected:
	virtual void BeginPlay() override;

private:	
	TSharedPtr<IWebSocket> WebSocket = nullptr;

	void OnSocketConnected() const;
	void OnSocketConnectionError(const FString& err) const;
	void OnSocketClosed(const int statusCode, const FString& reason, const bool bWasClean) const;
	void OnSocketMessageReceived(const FString& message);

	FString GenerateRandomString(int32 Length);

	FString ChatId;
	FORCEINLINE FString GetChatId() const { return ChatId; }
	FORCEINLINE void SetChatId(const FString& Value) { ChatId = Value; }

};
