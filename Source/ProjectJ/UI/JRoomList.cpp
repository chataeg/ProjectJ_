// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/JRoomList.h"
#include "UI/JRoomListItemData.h"
#include "../Network/JPacketHandler.h"


void UJRoomList::NativeContstruct()
{
	UE_LOG(LogTemp, Warning, TEXT("Roomlist Native Const"));
}

void UJRoomList::NativeDestruct()
{
	UE_LOG(LogTemp, Warning, TEXT("Roomlist Native Destruct"));
	// UJPacketHandler::Packet_S_LOBBY_REFRESH_ROOM_Delegate.Unbind();
	//UJPacketHandler::Packet_S_LOBBY_ENTER_ROOM_Delegate.Unbind();
}

void UJRoomList::NativeOnInitialized()
{
	UE_LOG(LogTemp, Warning, TEXT("Roomlist Native Init"));
	UJPacketHandler::Packet_S_LOBBY_REFRESH_ROOM_Delegate.BindUObject(this, &UJRoomList::OnRecvRoomList);
	UJPacketHandler::Packet_S_LOBBY_ENTER_ROOM_Delegate.BindUObject(this, &UJRoomList::OnRecvEnterRoom);
}

void UJRoomList::UnBindAllDelegate()
{
	UJPacketHandler::Packet_S_LOBBY_REFRESH_ROOM_Delegate.Unbind();
	UJPacketHandler::Packet_S_LOBBY_ENTER_ROOM_Delegate.Unbind();
}

void UJRoomList::OnRequestRoomList()
{
	ProjectJ::C_LOBBY_REFRESH_ROOM SendPacket;

	if (auto JGameInst = Cast<UJGameInstance>(GetGameInstance()))
	{
		SendPacket.set_account_id(JGameInst->GetSession()->GetID());

		auto SendBuffer = UJPacketHandler::MakeSendBuffer(SendPacket);
		JGameInst->SendAsync(SendBuffer);

		UE_LOG(LogTemp, Warning, TEXT("RoomListRequestSend"));
	}
}

bool UJRoomList::OnRecvRoomList(UWorld* World, ProjectJ::S_LOBBY_REFRESH_ROOM& Packet, float Deltaseconds)
{
	UE_LOG(LogTemp, Warning, TEXT("RoomListRecved"));
	if (RoomListView)
	{
		RoomListView->ClearListItems();
		for (auto item : Packet.rooms())
		{
			UJRoomListItemData* NewItem = NewObject<UJRoomListItemData>(this, UJRoomListItemData::StaticClass());
			NewItem->RoomID = item.id();
			NewItem->RoomTitle = UTF8_TO_TCHAR(item.title().c_str());
			NewItem->RoomPlayerNum = item.number_of_player();
			NewItem->RoomState = item.state();

			RoomListView->AddItem(NewItem);

			UE_LOG(LogTemp, Warning, TEXT("RoomId : %d RoomTitle : %s"), NewItem->RoomID, *NewItem->RoomTitle)
		}
	}


	return true;
}

void UJRoomList::OnRequestEnterRoom(int32 RoomNum)
{
	ProjectJ::C_LOBBY_ENTER_ROOM SendPacket;

	if (auto JGameInst = Cast<UJGameInstance>(GetGameInstance()))
	{
		SendPacket.set_account_id(JGameInst->GetSession()->GetID());
		SendPacket.set_room_id(RoomNum);


		auto SendBuffer = UJPacketHandler::MakeSendBuffer(SendPacket);
		JGameInst->SendAsync(SendBuffer);

		UE_LOG(LogTemp, Warning, TEXT("RoomEnterRequestSend RoomNum %d"), RoomNum);
	}
}

bool UJRoomList::OnRecvEnterRoom(UWorld* World, ProjectJ::S_LOBBY_ENTER_ROOM& Packet, float Deltaseconds)
{
	auto JGameInst = Cast<UJGameInstance>(GetGameInstance());
	auto JGameMode = Cast<AJGameMode>(GetWorld()->GetAuthGameMode());

	if (Packet.result())
	{
		JGameMode->SetPlayerState(EGameState::Room);
		JGameMode->SetRoomId(Packet.room_id());
		JGameMode->SetSlotIndex(Packet.slot_index());

		if (AJPlayerController* PlayerController = Cast<AJPlayerController>(GetOwningPlayer()))
		{
			PlayerController->RoomWidgetInstance = CreateWidget(PlayerController, PlayerController->RoomWidgetClass);
			if (PlayerController->RoomWidgetInstance)
			{
				PlayerController->RoomWidgetInstance->AddToViewport();
			}

			if (PlayerController->LobbyWidgetInstance)
			{
				PlayerController->LobbyWidgetInstance->RemoveFromParent();
			}
		}
	}

	return true;
}
