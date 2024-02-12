// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerCharacterComponent.h"
#include "SpawnerApiSubsystem.h"

// Sets default values for this component's properties
USpawnerCharacterComponent::USpawnerCharacterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void USpawnerCharacterComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnerApiSubsystem = GetWorld()->GetSubsystem<USpawnerApiSubsystem>();
	
	if (CharacterId.IsEmpty())
	{
		return;
	}
	if (!ensure(SpawnerApiSubsystem.IsValid()))
	{
		return;
	}
	SpawnerApiSubsystem->RegisterSceneCharacter(this);

}


// Called every frame
void USpawnerCharacterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

