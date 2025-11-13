// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/JGameInstanceSubsystem.h"
#include "Network/JPacketHandler.h"

void UJGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	SendBufferManager = NewObject<UJSendBufferManager>();
}

void UJGameInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();
}
