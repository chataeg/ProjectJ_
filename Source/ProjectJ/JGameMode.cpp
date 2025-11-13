// Fill out your copyright notice in the Description page of Project Settings.


#include "JGameMode.h"
#include "JInGameMode.h"
#include "GameFramework/GameUserSettings.h"
#include "Player/JPlayerController.h"

AJGameMode::AJGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder
		// (TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
		(TEXT("/Script/CoreUObject.Class'/Script/ProjectJ.JChaserCharacter'"));


	PlayerControllerClass = AJPlayerController::StaticClass();
	PlayerStateClass = AJInGamePlayerState::StaticClass();

	//JGameMode = Cast<AJGameMode>(GetWorld()->GetAuthGameMode());
	//JGameInst = Cast<UJGameInstance>(GetGameInstance());

	UE_LOG(LogTemp, Warning, TEXT("JGameMode Constructor"));

	//Cast<AJInGameMode>(GetWorld()->GetAuthGameMode())->SetJGameInstance(Cast<UJGameInstance>(GetGameInstance()));
}

EGameState AJGameMode::GetPlayerState()
{
	return PlayerState;
}

void AJGameMode::SetPlayerState(EGameState Input)
{
	PlayerState = Input;
}

void AJGameMode::SetSlotIndex(int32 input)
{
	Slot_Index = input;
}

int32 AJGameMode::GetSlotIndex()
{
	return Slot_Index;
}

void AJGameMode::SetRoomId(int32 input)
{
	RoomId = input;
}

int32 AJGameMode::GetRoomId()
{
	return RoomId;
}
