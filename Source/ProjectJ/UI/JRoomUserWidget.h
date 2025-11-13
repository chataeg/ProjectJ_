// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Network/JGameInstance.h"
#include "../JGameMode.h"
#include "Blueprint/UserWidget.h"
#include "../Player/JInGamePlayerState.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "JRoomUserWidget.generated.h"

// 대기실 위젯 구현부
// 최초 작성자 : 차태관
// 수정자 : 박별
// 최종 수정일 : 2023-11-20
UCLASS()
class PROJECTJ_API UJRoomUserWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void NativeDestruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

	void UnBindAllDelegate();


	void UpdateRoomInfo(const ProjectJ::RoomInfo& Packet);
	void UpdateOtherEnterRoom(const int32 RoomId, const int32 SlotIndex, const std::string NickName);
	void UpdateOtherLeaveRoom(const int32 RoomId, const int32 SlotIndex);
	void UpdateReadyOrNot(const int32 SlotIndex, const bool IsReady);

	UFUNCTION(BlueprintCallable)
	void OnRequestReady();

	bool OnRecvReady(UWorld* World, ProjectJ::S_ROOM_READY& Packet, float Deltaseconds);

	UFUNCTION(BlueprintCallable)
	void OnRequestLeaveRoom();

	bool OnRecvLeaveRoom(UWorld* World, ProjectJ::S_ROOM_LEAVE& Packet, float Deltaseconds);

	UFUNCTION(BlueprintCallable)
	void OnRequestReady2Recv();

	bool OnRecvRoomInfo(UWorld* World, ProjectJ::S_ROOM_INFO& Packet, float Deltaseconds);
	bool OnRecvRoomChat(UWorld* World, ProjectJ::S_ROOM_CHAT& Packet, float Deltaseconds);
	bool OnRecvOtherEnter(UWorld* World, ProjectJ::S_ROOM_OTHER_ENTER& Packet, float Deltaseconds);
	bool OnRecvOtherLeave(UWorld* World, ProjectJ::S_ROOM_OTHER_LEAVE& Packet, float Deltaseconds);

	bool OnRecvStandby(UWorld* World, ProjectJ::S_ROOM_STANDBY_MATCH& Packet, float Deltaseconds);
	bool OnRecvStartMatch(UWorld* World, ProjectJ::S_ROOM_START_MATCH& Packet, float Deltaseconds);

	AGameModeBase* GetGameModeBase() const { return Cast<AJGameMode>(GetWorld()->GetAuthGameMode()); }
	UJGameInstance* GetGameInstance() const { return Cast<UJGameInstance>(GetGameModeBase()->GetGameInstance()); }
	UUserWidget* GetRoomChatBP() const { return Cast<UUserWidget>(GetWidgetFromName(TEXT("BP_Chat"))); }

	void SetPlayerNickname(int32 SlotIndex, FText Nickname);
	void SetPlayerReady(int32 SlotIndex, bool IsReady);
	void ClearPlayerInfo(int32 SlotIndex);

protected:
	bool IsStandbyBegin = false;
	int32 MySlotIndex = -1;

	UPROPERTY()
	UUserWidget* ChatWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UUserWidget*> PlayerInfoWidgets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* ReadyButtonWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* RoomInfoTextWidget;
};
