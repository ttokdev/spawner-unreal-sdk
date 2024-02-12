// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerPlayerComponent.h"
#include "SpawnerTypes.h"

// Sets default values for this component's properties
USpawnerPlayerComponent::USpawnerPlayerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpawnerPlayerComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnerApiSubsystem = GetWorld()->GetSubsystem<USpawnerApiSubsystem>();

	if (PlayerId.IsEmpty())
	{
		return;
	}
	if (!ensure(SpawnerApiSubsystem.IsValid()))
	{
		return;
	}
	FSpawnerPlayerInfo PlayerInfo;
	PlayerInfo.Id = PlayerId;
	PlayerInfo.Name = PlayerName;
	PlayerInfo.Description = PlayerDescription;
	PlayerInfo.AgeGroup = PlayerAgeGroup;
	PlayerInfo.Gender = PlayerGender;

	SpawnerApiSubsystem->RegisterPlayerInfo(PlayerInfo);
	
}


// Called every frame
void USpawnerPlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

