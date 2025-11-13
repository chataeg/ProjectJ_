// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "Internationalization/Text.h"
#include "Components/ProgressBar.h"
#include "../Player/JInGamePlayerState.h"
#include "GameFramework/Actor.h"
#include "../Network/JGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "../Network/JPacketHandler.h"
#include "JInGameUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJ_API UJInGameUserWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void NativeContsruct();
	virtual void NativeDestruct() override;


	UFUNCTION(BlueprintCallable)
	void OnUpdateUIInfo();

	void Init(ProjectJ::S_MATCH_ALL_READY_TO_RECEIVE& Packet);

	float CalHp(const ProjectJ::MatchPlayerState State);

	bool IsTickEnd(uint64 input);


	UPROPERTY()
	FTimerHandle Timer;

	UJGameInstance* JGameInst;
	UTextBlock* TimeBlock;

	uint64 EndTick;
	uint64 CurrentTick;

	uint64 Time_m;
	uint64 Time_s;


protected:
	UProgressBar* Fug1HpBar;
	UProgressBar* Fug2HpBar;
	UProgressBar* Fug3HpBar;

	AJInGamePlayerState* LocalCurrentState;
};
