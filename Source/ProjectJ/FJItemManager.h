// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs.h"
#include "Network/Protocol/Message.pb.h"
#include "Player/JInGamePlayerState.h"

// 로컬 아이템 관리자
// 최초 작성자 : 김기홍
// 수정자 :
// 최종 수정일 : 2023-11-23
class UWorld;
class PROJECTJ_API FJItemManager
{
public:
	FJItemManager(UWorld* World);

	void GenerateItems(ProjectJ::S_MATCH_ITEM_GENERATED& Packet);

private:
	UWorld* World;
	AJInGamePlayerState* PlayerState;

	// Convert To Unreal data structure
	FVector GetFVector(ProjectJ::Vector);
	FRotator GetFRotator(ProjectJ::Rotator);

	// Find Blueprint By ID
	UBlueprint* GetBluprintClass(int itemID);

	// 오브젝트를 생성하는 함수
	AJMoveable* CreateLocalObject(UBlueprint* BlueprintClass, FItemData ItemData);

};
