// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <memory>

#include "CoreMinimal.h"
#include "../Network/JGameInstance.h"
#include "GameFramework/PlayerState.h"
#include "Pawn/JMoveable.h"
#include "JInGamePlayerState.generated.h"



/**
 * 
 */
UCLASS()
class PROJECTJ_API AJInGamePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AJInGamePlayerState();

	void AddCharacter(AJCharacterBase* Character, int Index);
	void AddItem(AJMoveable* Item, int Index);

	AJCharacterBase* GetCharacter(int Index);
	AJMoveable* GetItem(int Index);

	UJGameInstance* JGameInst;
	UWorld* World;

	UPROPERTY()
	TMap<int, AJCharacterBase*> Characters;
	UPROPERTY()
	TMap<int, AJMoveable*> Items;

	ProjectJ::MatchPlayerState CurrentState;




};