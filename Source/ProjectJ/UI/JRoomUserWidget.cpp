// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/JRoomUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "../Network/JPacketHandler.h"
#include "Components/Image.h"
#include "Player/JPlayerController.h"

void UJRoomUserWidget::NativeDestruct()
{
	Super::NativeDestruct();

	UE_LOG(LogTemp, Warning, TEXT("Room Native Destruct"));
	IsStandbyBegin = false;
	MySlotIndex = -1;

	/*
	UJPacketHandler::Packet_S_ROOM_READY_Delegate.Unbind();
	UJPacketHandler::Packet_S_ROOM_LEAVE_Delegate.Unbind();
	UJPacketHandler::Packet_S_ROOM_OTHER_ENTER_Delegate.Unbind();
	UJPacketHandler::Packet_S_ROOM_OTHER_LEAVE_Delegate.Unbind();
	UJPacketHandler::Packet_S_ROOM_INFO_Delegate.Unbind();
	UJPacketHandler::Packet_S_ROOM_STANDBY_MATCH_Delegate.Unbind();
	UJPacketHandler::Packet_S_ROOM_START_MATCH_Delegate.Unbind();
	   */
}

void UJRoomUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Warning, TEXT("Room Native construct"));

	ChatWidget = Cast<UUserWidget>(GetWidgetFromName("BP_Chat"));
	check(ChatWidget);

	if (auto ChatLog = Cast<UScrollBox>(ChatWidget->GetWidgetFromName("ChatLog")))
	{
		ChatLog->ClearChildren();
	}

	PlayerInfoWidgets.Init(nullptr, 4);

	PlayerInfoWidgets[0] = Cast<UUserWidget>(GetWidgetFromName("ChaserInfo"));
	check(PlayerInfoWidgets[0]);

	PlayerInfoWidgets[1] = Cast<UUserWidget>(GetWidgetFromName("FugitiveFirstInfo"));
	check(PlayerInfoWidgets[1]);

	PlayerInfoWidgets[2] = Cast<UUserWidget>(GetWidgetFromName("FugitiveSecondInfo"));
	check(PlayerInfoWidgets[2]);

	PlayerInfoWidgets[3] = Cast<UUserWidget>(GetWidgetFromName("FugitiveThirdInfo"));
	check(PlayerInfoWidgets[3]);

	ReadyButtonWidget = Cast<UUserWidget>(GetWidgetFromName("ReadyBtn"));
	check(ReadyButtonWidget);

	RoomInfoTextWidget = Cast<UTextBlock>(GetWidgetFromName("RoomInfoText"));
	check(RoomInfoTextWidget);

	OnRequestReady2Recv();
}

void UJRoomUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UE_LOG(LogTemp, Warning, TEXT("Room Native init"));
	UJPacketHandler::Packet_S_ROOM_READY_Delegate.BindUObject(this, &UJRoomUserWidget::OnRecvReady);
	UJPacketHandler::Packet_S_ROOM_LEAVE_Delegate.BindUObject(this, &UJRoomUserWidget::OnRecvLeaveRoom);
	UJPacketHandler::Packet_S_ROOM_OTHER_ENTER_Delegate.BindUObject(this, &UJRoomUserWidget::OnRecvOtherEnter);
	UJPacketHandler::Packet_S_ROOM_OTHER_LEAVE_Delegate.BindUObject(this, &UJRoomUserWidget::OnRecvOtherLeave);
	UJPacketHandler::Packet_S_ROOM_INFO_Delegate.BindUObject(this, &UJRoomUserWidget::OnRecvRoomInfo);
	UJPacketHandler::Packet_S_ROOM_STANDBY_MATCH_Delegate.BindUObject(this, &UJRoomUserWidget::OnRecvStandby);
	UJPacketHandler::Packet_S_ROOM_START_MATCH_Delegate.BindUObject(this, &UJRoomUserWidget::OnRecvStartMatch);
	UJPacketHandler::Packet_S_ROOM_CHAT_Delegate.BindUObject(this, &UJRoomUserWidget::OnRecvRoomChat);
}

void UJRoomUserWidget::UnBindAllDelegate()
{
	UJPacketHandler::Packet_S_ROOM_READY_Delegate.Unbind();
	UJPacketHandler::Packet_S_ROOM_LEAVE_Delegate.Unbind();
	UJPacketHandler::Packet_S_ROOM_OTHER_ENTER_Delegate.Unbind();
	UJPacketHandler::Packet_S_ROOM_OTHER_LEAVE_Delegate.Unbind();
	UJPacketHandler::Packet_S_ROOM_INFO_Delegate.Unbind();
	UJPacketHandler::Packet_S_ROOM_STANDBY_MATCH_Delegate.Unbind();
	UJPacketHandler::Packet_S_ROOM_START_MATCH_Delegate.Unbind();
}


void UJRoomUserWidget::UpdateRoomInfo(const ProjectJ::RoomInfo& Packet)
{
	auto JGameMode = Cast<AJGameMode>(GetWorld()->GetAuthGameMode());
	auto JGameInst = Cast<UJGameInstance>(GetGameInstance());

	if (Packet.has_chaser())
	{
		PlayerInfoWidgets[0]->SetVisibility(ESlateVisibility::Visible);
		FString Nickname = UTF8_TO_TCHAR(Packet.chaser().player().nickname().c_str());
		int32 AccountID = Packet.chaser().player().account_id();

		if (AccountID == JGameInst->GetSession()->GetID())
		{
			MySlotIndex = 0;
		}

		SetPlayerNickname(0, FText::FromString(Nickname));
		SetPlayerReady(0, Packet.chaser().is_ready());
	}

	if (Packet.has_fugitive_first())
	{
		PlayerInfoWidgets[1]->SetVisibility(ESlateVisibility::Visible);
		FString Nickname = UTF8_TO_TCHAR(Packet.fugitive_first().player().nickname().c_str());
		int32 AccountID = Packet.fugitive_first().player().account_id();

		if (AccountID == JGameInst->GetSession()->GetID())
		{
			MySlotIndex = 1;
		}

		SetPlayerNickname(1, FText::FromString(Nickname));
		SetPlayerReady(1, Packet.fugitive_first().is_ready());
	}

	if (Packet.has_fugitive_second())
	{
		PlayerInfoWidgets[2]->SetVisibility(ESlateVisibility::Visible);
		FString Nickname = UTF8_TO_TCHAR(Packet.fugitive_second().player().nickname().c_str());
		int32 AccountID = Packet.fugitive_second().player().account_id();

		if (AccountID == JGameInst->GetSession()->GetID())
		{
			MySlotIndex = 2;
		}

		SetPlayerNickname(2, FText::FromString(Nickname));
		SetPlayerReady(2, Packet.fugitive_second().is_ready());
	}

	if (Packet.has_fugitive_third())
	{
		PlayerInfoWidgets[3]->SetVisibility(ESlateVisibility::Visible);
		FString Nickname = UTF8_TO_TCHAR(Packet.fugitive_third().player().nickname().c_str());
		int32 AccountID = Packet.fugitive_third().player().account_id();

		if (AccountID == JGameInst->GetSession()->GetID())
		{
			MySlotIndex = 3;
		}

		SetPlayerNickname(3, FText::FromString(Nickname));
		SetPlayerReady(3, Packet.fugitive_third().is_ready());
	}

	RoomInfoTextWidget->SetText(FText::FromString("#" + FString::FromInt(Packet.room_id()) + " " + UTF8_TO_TCHAR(Packet.title().c_str())));

	UE_LOG(LogTemp, Warning, TEXT("UpdateRoomInfo"));
}

void UJRoomUserWidget::UpdateOtherEnterRoom(const int32 RoomId, const int32 SlotIndex, const std::string NickName)
{
	auto JGameMode = Cast<AJGameMode>(GetWorld()->GetAuthGameMode());
	auto JGameInst = Cast<UJGameInstance>(GetGameInstance());

	SetPlayerNickname(SlotIndex, FText::FromString(UTF8_TO_TCHAR(NickName.c_str())));
	SetPlayerReady(SlotIndex, false);

	UE_LOG(LogTemp, Warning, TEXT("UpdateOtherEnterRoom"));
}

void UJRoomUserWidget::UpdateOtherLeaveRoom(const int32 RoomId, const int32 SlotIndex)
{
	auto JGameMode = Cast<AJGameMode>(GetWorld()->GetAuthGameMode());
	auto JGameInst = Cast<UJGameInstance>(GetGameInstance());

	ClearPlayerInfo(SlotIndex);

	UE_LOG(LogTemp, Warning, TEXT("UpdateOtherLeaveRoom"));
}

void UJRoomUserWidget::UpdateReadyOrNot(const int32 SlotIndex, const bool IsReady)
{
	SetPlayerReady(SlotIndex, IsReady);

	UE_LOG(LogTemp, Warning, TEXT("UpdateReadyOrNot %d: %d"), SlotIndex, IsReady);
}

void UJRoomUserWidget::OnRequestReady()
{
	ProjectJ::C_ROOM_READY SendPacket;

	if (auto JGameInst = Cast<UJGameInstance>(GetGameInstance()))
	{
		auto JGameMode = Cast<AJGameMode>(GetWorld()->GetAuthGameMode());
		SendPacket.set_account_id(JGameInst->GetSession()->GetID());
		SendPacket.set_room_id(JGameMode->GetRoomId());
		SendPacket.set_slot_index(JGameMode->GetSlotIndex());

		auto SendBuffer = UJPacketHandler::MakeSendBuffer(SendPacket);
		JGameInst->SendAsync(SendBuffer);
		UE_LOG(LogTemp, Warning, TEXT("RoomReadyRequestSend"));
	}
}

bool UJRoomUserWidget::OnRecvReady(UWorld* World, ProjectJ::S_ROOM_READY& Packet, float Deltaseconds)
{
	UpdateReadyOrNot(Packet.slot_index(), Packet.is_ready());

	UE_LOG(LogTemp, Warning, TEXT("OnRecvReady and updated"));
	return true;
}

bool UJRoomUserWidget::OnRecvOtherEnter(UWorld* World, ProjectJ::S_ROOM_OTHER_ENTER& Packet, float Deltaseconds)
{
	UpdateOtherEnterRoom(Packet.room_id(), Packet.slot_index(), Packet.other().nickname());

	UE_LOG(LogTemp, Warning, TEXT("OnRecvOtherEnter and Update"));

	return true;
}

bool UJRoomUserWidget::OnRecvOtherLeave(UWorld* World, ProjectJ::S_ROOM_OTHER_LEAVE& Packet, float Deltaseconds)
{
	UpdateOtherLeaveRoom(Packet.room_id(), Packet.slot_index());
	UE_LOG(LogTemp, Warning, TEXT("OnRecvOtherLeave and Update"));

	return true;
}

bool UJRoomUserWidget::OnRecvStandby(UWorld* World, ProjectJ::S_ROOM_STANDBY_MATCH& Packet, float Deltaseconds)
{
	if (auto ChatLog = Cast<UScrollBox>(GetRoomChatBP()->GetWidgetFromName("ChatLog")))
	{
		if (auto PlayerController = Cast<AJPlayerController>(GetOwningPlayer()))
		{
			if (!IsStandbyBegin)
			{
				IsStandbyBegin = true;

				PlayerController->OnRecvChat(ChatLog, FText::FromString(UTF8_TO_TCHAR("모두 준비 완료. 게임을 시작합니다.")), true);
			}

			PlayerController->OnRecvChat(ChatLog, FText::FromString(FString::FromInt(Packet.count()) + "..."), true);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("OnRecvStandy : %d"), Packet.count());

	return true;
}

bool UJRoomUserWidget::OnRecvStartMatch(UWorld* World, ProjectJ::S_ROOM_START_MATCH& Packet, float Deltaseconds)
{
	if (Packet.start())
	{
		auto JGameInst = Cast<UJGameInstance>(GetGameInstance());
		auto JGameMode = Cast<AJGameMode>(GetWorld()->GetAuthGameMode());

		JGameMode->SetPlayerState(EGameState::InGame);

		this->RemoveFromParent();

		FOutputDeviceNull pAR;

		UGameplayStatics::OpenLevel(JGameInst, TEXT("InGame_Shin"));


		UE_LOG(LogTemp, Warning, TEXT("OnRecvStartMatch"));
	}
	return true;
}

void UJRoomUserWidget::SetPlayerNickname(int32 SlotIndex, FText Nickname)
{
	if (auto NicknameText = Cast<UTextBlock>(PlayerInfoWidgets[SlotIndex]->GetWidgetFromName("NicknameText")))
	{
		NicknameText->SetText(Nickname);

		if (SlotIndex == MySlotIndex)
		{
			NicknameText->SetColorAndOpacity(FColor::FromHex("ffc000"));
		}
	}
}

void UJRoomUserWidget::SetPlayerReady(int32 SlotIndex, bool IsReady)
{
	if (auto ReadyImage = Cast<UImage>(PlayerInfoWidgets[SlotIndex]->GetWidgetFromName("ReadyImage")))
	{
		ReadyImage->SetVisibility(IsReady ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

		if (SlotIndex == MySlotIndex)
		{
			if (auto ReadyButtonText = Cast<UTextBlock>(ReadyButtonWidget->GetWidgetFromName("ButtonTextBlock")))
			{
				ReadyButtonText->SetText(FText::FromString(UTF8_TO_TCHAR(IsReady ? "준비취소" : "준비")));
			}
		}
	}
}

void UJRoomUserWidget::ClearPlayerInfo(int32 SlotIndex)
{
	PlayerInfoWidgets[SlotIndex]->SetVisibility(ESlateVisibility::Hidden);
}

void UJRoomUserWidget::OnRequestLeaveRoom()
{
	ProjectJ::C_ROOM_LEAVE SendPacket;

	if (auto JGameInst = Cast<UJGameInstance>(GetGameInstance()))
	{
		auto JGameMode = Cast<AJGameMode>(GetWorld()->GetAuthGameMode());

		SendPacket.set_account_id(JGameInst->GetSession()->GetID());
		SendPacket.set_room_id(JGameMode->GetRoomId());
		SendPacket.set_slot_index(JGameMode->GetSlotIndex());

		auto SendBuffer = UJPacketHandler::MakeSendBuffer(SendPacket);
		JGameInst->SendAsync(SendBuffer);
	}
}

bool UJRoomUserWidget::OnRecvLeaveRoom(UWorld* World, ProjectJ::S_ROOM_LEAVE& Packet, float Deltaseconds)
{
	auto JGameInst = Cast<UJGameInstance>(GetGameInstance());
	auto JGameMode = Cast<AJGameMode>(GetWorld()->GetAuthGameMode());

	if (Packet.result())
	{
		UE_LOG(LogTemp, Warning, TEXT("RoomLeaveRequestRecved RoomId ; %d"), JGameMode->GetRoomId());
		JGameMode->SetPlayerState(EGameState::Lobby);

		Cast<UScrollBox>(GetRoomChatBP()->GetWidgetFromName("ChatLog"))->ClearChildren();
		this->RemoveFromParent();

		if (AJPlayerController* PlayerController = Cast<AJPlayerController>(GetOwningPlayer()))
		{
			if (PlayerController->LobbyWidgetInstance == nullptr)
			{
				PlayerController->LobbyWidgetInstance = CreateWidget(PlayerController, PlayerController->LobbyWidgetClass);
			}

			if (PlayerController->LobbyWidgetInstance)
			{
				PlayerController->LobbyWidgetInstance->AddToViewport();
			}
		}
	}
	return true;
}

void UJRoomUserWidget::OnRequestReady2Recv()
{
	ProjectJ::C_ROOM_READY_TO_RECEIVE SendPacket;

	if (auto JGameInst = Cast<UJGameInstance>(GetGameInstance()))
	{
		auto JGameMode = Cast<AJGameMode>(GetWorld()->GetAuthGameMode());

		SendPacket.set_account_id(JGameInst->GetSession()->GetID());
		SendPacket.set_room_id(JGameMode->GetRoomId());
		SendPacket.set_slot_index(JGameMode->GetSlotIndex());


		auto SendBuffer = UJPacketHandler::MakeSendBuffer(SendPacket);
		JGameInst->SendAsync(SendBuffer);
		UE_LOG(LogTemp, Warning, TEXT("SendReady2Recv"));
	}
}

bool UJRoomUserWidget::OnRecvRoomInfo(UWorld* World, ProjectJ::S_ROOM_INFO& Packet, float Deltaseconds)
{
	UpdateRoomInfo(Packet.info());
	UE_LOG(LogTemp, Warning, TEXT("RecvRoomInfo and Updated"));


	return true;
}

bool UJRoomUserWidget::OnRecvRoomChat(UWorld* World, ProjectJ::S_ROOM_CHAT& Packet, float Deltaseconds)
{
	if (auto ChatLog = Cast<UScrollBox>(ChatWidget->GetWidgetFromName("ChatLog")))
	{
		if (auto PlayerController = Cast<AJPlayerController>(GetOwningPlayer()))
		{
			FString Nickname = UTF8_TO_TCHAR(Packet.nickname().c_str());
			FString ChatMessage = UTF8_TO_TCHAR(Packet.chat().c_str());

			PlayerController->OnRecvChat(ChatLog, FText::FromString("[" + Nickname + "] " + ChatMessage));
		}
	}

	return true;
}
