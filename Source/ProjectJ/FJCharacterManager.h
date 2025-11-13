// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/JCharacterBase.h"
#include "Character/JPlayableCharacter.h"
#include "Player/JInGamePlayerState.h"

// 캐릭터 관리자 (캐릭터 소환 및 아이디 할당)
// 최초 작성자 : 김기홍
// 수정자 :
// 최종 수정일 : 2023-11-23
class PROJECTJ_API FJCharacterManager
{
public:
	FJCharacterManager(UWorld* World);
	~FJCharacterManager();

	void InitCharacters(ProjectJ::S_MATCH_ALL_READY_TO_RECEIVE& Packet);

private:
	UWorld* World;
	AJInGamePlayerState* PlayerState;

	// 캐릭터 생성
	void SpawnPlayer(UClass*, ProjectJ::PlayerInfo);

	// NPC 생성
	void SpawnScale(UBlueprint*, ProjectJ::MatchInitInfo_ScaleInitInfo);

	// 오브젝트를 생성하는 함수
	AJCharacterBase* CreateLocalObject(UClass* Class, FVector Location, FRotator Rotator, int ID);

	// Convert To Unreal data structure
	FVector GetFVector(ProjectJ::Vector);
	FRotator GetFRotator(ProjectJ::Rotator);
};
