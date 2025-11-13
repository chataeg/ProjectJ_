// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/JInGamePlayerState.h"
#include "ProjectJGameModeBase.h"
#include "JGameMode.generated.h"


// 접속 시 로그인 창부터 활용할 게임모드
// 최초 작성자 : 차태관
// 수정자 :
// 최종 수정일 : 2023-11-20
UENUM(BlueprintType)
enum class EGameState : uint8
{
	Login UMETA(DisplayName = "Login"),
	Lobby UMETA(DisplayName = "Lobby"),
	Room UMETA(DisplayName = "Room"),
	InGame UMETA(DisplayName = "InGame")
};

struct FPlayer
{
	int32 account_id;
	FString nickname;
};

struct FPlayerSlot
{
	FPlayer player;
	bool is_ready;
};

struct FRoomInfo
{
	int32 room_id;
	FString title;
	FPlayerSlot chaser;
	FPlayerSlot fugitive_first;
	FPlayerSlot fugitive_second;
	FPlayerSlot fugitive_third;
};

UCLASS()
class PROJECTJ_API AJGameMode : public AProjectJGameModeBase
{
	GENERATED_BODY()


public:
	AJGameMode();

	EGameState GetPlayerState();
	void SetPlayerState(EGameState Input);

	void SetSlotIndex(int32 input);
	int32 GetSlotIndex();

	void SetRoomId(int32 input);
	int32 GetRoomId();

protected:
	EGameState PlayerState;

	int32 RoomId;

	int32 Slot_Index;
};
