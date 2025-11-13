// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Network/JGameInstance.h"
#include "../UI/JRoomList.h"
#include "../JGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "JLobbyUserWidget.generated.h"

// 로비 위젯 구현부
// 최초 작성자 : 차태관
// 수정자 : 박별
// 최종 수정일 : 2023-10-28
UCLASS()
class PROJECTJ_API UJLobbyUserWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UJLobbyUserWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnInitialized() override;

	void UnBindAllDelegate();

	UFUNCTION(BlueprintCallable)
	void OnRequestCreateRoom(const FString& title);
	bool OnRecvLobbyChat(UWorld* World, ProjectJ::S_LOBBY_CHAT& Packet, float Deltaseconds);
	bool OnRecvCreateRoom(UWorld* World, ProjectJ::S_LOBBY_CREATE_ROOM& Packet, float Deltaseconds);

protected:
	UPROPERTY()
	UTextBlock* SessionNickname;

	UPROPERTY()
	UUserWidget* ChatWidget;
};
