// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/JInGamePlayerState.h"

AJInGamePlayerState::AJInGamePlayerState()
{
	Items.Reserve(150);
	Characters.Reserve(10);
	CurrentState = ProjectJ::NONE;
}

void AJInGamePlayerState::AddCharacter(AJCharacterBase* Character, int Index)
{
	Characters.Add(Index, Character);
}

void AJInGamePlayerState::AddItem(AJMoveable* Item, int Index)
{
	Items.Add(Index, Item);
}

AJCharacterBase* AJInGamePlayerState::GetCharacter(int Index)
{
	auto FoundValue = Characters.Find(Index);

	if (FoundValue)
	{
		return *FoundValue;
	}
	return nullptr;
}

AJMoveable* AJInGamePlayerState::GetItem(int Index)
{
	auto FoundValue = Items.Find(Index);

	if (FoundValue)
	{
		return *FoundValue;
	}
	return nullptr;
}
