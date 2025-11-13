// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Network/JPacketHandler.h"
#include "ProjectJGameModeBase.generated.h"


UCLASS()
class PROJECTJ_API AProjectJGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectJGameModeBase();
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	FTimerHandle Handle;
};
