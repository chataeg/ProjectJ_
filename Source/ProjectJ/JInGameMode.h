// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FJCharacterManager.h"
#include "FJItemManager.h"
#include "../ProjectJ/UI/JInGameUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectJGameModeBase.h"
#include "JInGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJ_API AJInGameMode : public AProjectJGameModeBase
{
	GENERATED_BODY()
public:
	AJInGameMode();
	// TODO : 게임 종료시 아이템 및 플레이어, NPC 관련 메모리 정리

	void OnRequestReady2Recv();


	///////////////////////////////////////////////////
	/// Match - Start
	void OnRequestMatchStart();
	bool OnRecvMatchStart(UWorld* World, ProjectJ::S_MATCH_START& Packet, float Deltaseconds);

	///////////////////////////////////////////////////
	/// Match - End


	///////////////////////////////////////////////////
	/// Recieve Player Info
	bool OnRecvPlayerInfo(UWorld* World, ProjectJ::S_MATCH_ALL_READY_TO_RECEIVE& Packet, float Deltaseconds);

	///////////////////////////////////////////////////
	/// Generate - Item
	bool OnRecvGenerateItems(UWorld* World, ProjectJ::S_MATCH_ITEM_GENERATED& Packet, float Deltaseconds);

	///////////////////////////////////////////////////
	/// Response - Inventory
	bool OnRecvAddItem(UWorld* World, ProjectJ::S_MATCH_ITEM_SOMEONE_PICKUP& Packet, float Deltaseconds);
	bool OnRecvMoveItem(UWorld* World, ProjectJ::S_MATCH_ITEM_SOMEONE_MOVE& Packet, float Deltaseconds);
	bool OnRecvDropItem(UWorld* World, ProjectJ::S_MATCH_ITEM_SOMEONE_DROP& Packet, float Deltaseconds);

	///////////////////////////////////////////////////
	/// Response - Scale
	bool OnRecvScaleOnChanged(UWorld* World, ProjectJ::S_MATCH_SCALE_ON_CHANGED& Packet, float Deltaseconds);

	///////////////////////////////////////////////////
	/// Response - Player
	bool OnRecvChaserAttack(UWorld* World, ProjectJ::S_MATCH_CHASER_ATTACK& Packet, float Deltaseconds);

	bool OnRecvChaserHit(UWorld* World, ProjectJ::S_MATCH_CHASER_HIT& Packet, float Deltaseconds);


	///////////////////////////////////////////////////
	/// 구현 전
	//bool OnRecvChaserInstallCCTV(UWorld* World, ProjectJ::S_MATCH_CHASER_INSTALL_CCTV& Packet, float Deltaseconds);

	//bool OnRecvFugitiveEscape(UWorld* World, ProjectJ::S_MATCH_FUGITIVE_ESCAPE& Packet, float Deltaseconds);

	//UJGameInstance* GetJGameInstance();
	//void SetJGameInstance(UJGameInstance* input);


protected:
	bool IsItemGenerated;
	bool IsCharacterGenerated;
	int YourPlayerIndex;

	TSharedPtr<FJItemManager> ItemManager;
	TSharedPtr<FJCharacterManager> CharacterManager;


	//UPROPERTY(BlueprintReadWrite)
	//UJGameInstance* JGameInst;

	UPROPERTY()
	TSubclassOf<class UUserWidget> InGameWidgetClass;

	UPROPERTY()
	class UJInGameUserWidget* InGameWidgetInstance;
};
