// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ListView.h"
#include "JRoomUserWidget.h"
#include "../Player/JPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "JRoomList.generated.h"

// 방 목록 위젯 구현부
// 최초 작성자 : 차태관
// 수정자 : 박별
// 최종 수정일 : 2023-11-20
UCLASS()
class PROJECTJ_API UJRoomList : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void NativeContstruct();
	virtual void NativeDestruct() override;
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable)
	void OnRequestRoomList();

	bool OnRecvRoomList(UWorld* World, ProjectJ::S_LOBBY_REFRESH_ROOM& Packet, float Deltaseconds);

	UFUNCTION(BlueprintCallable)
	void OnRequestEnterRoom(int32 RoomNum);

	bool OnRecvEnterRoom(UWorld* World, ProjectJ::S_LOBBY_ENTER_ROOM& Packet, float Deltaseconds);

	void UnBindAllDelegate();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UListView> RoomListView;

	UPROPERTY(EditDefaultsOnly, Category = Events)
	FOnPointerEvent OnMouseDoubleClickEvent;
};
