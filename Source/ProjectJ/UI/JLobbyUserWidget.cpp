// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/JLobbyUserWidget.h"
#include "../Network/JPacketHandler.h"


UJLobbyUserWidget::UJLobbyUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UJLobbyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Warning, TEXT("Lobby Native Const"));

	if (auto JGameInst = Cast<UJGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		SessionNickname = Cast<UTextBlock>(WidgetTree->FindWidget("NicknameTextBlock"));
		FText Nickname = FText::FromString(*JGameInst->GetSession()->GetNickname());
		SessionNickname->SetText(Nickname);
	}

	ChatWidget = Cast<UUserWidget>(GetWidgetFromName("BP_Chat"));
	check(ChatWidget);

	if (auto ChatLog = Cast<UScrollBox>(ChatWidget->GetWidgetFromName("ChatLog")))
	{
		ChatLog->ClearChildren();
	}
}

void UJLobbyUserWidget::NativeDestruct()
{
	Super::NativeDestruct();
	UE_LOG(LogTemp, Warning, TEXT("Lobby  Native dest"));

	//UJPacketHandler::Packet_S_LOBBY_CREATE_ROOM_Delegate.Unbind();
}

void UJLobbyUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UJPacketHandler::Packet_S_LOBBY_CREATE_ROOM_Delegate.BindUObject(this, &UJLobbyUserWidget::OnRecvCreateRoom);
	UJPacketHandler::Packet_S_LOBBY_CHAT_Delegate.BindUObject(this, &UJLobbyUserWidget::OnRecvLobbyChat);
}

void UJLobbyUserWidget::UnBindAllDelegate()
{
	UJPacketHandler::Packet_S_LOBBY_CREATE_ROOM_Delegate.Unbind();
}

void UJLobbyUserWidget::OnRequestCreateRoom(const FString& title)
{
	ProjectJ::C_LOBBY_CREATE_ROOM SendPacket;

	if (auto JGameInst = Cast<UJGameInstance>(GetGameInstance()))
	{
		SendPacket.set_account_id(JGameInst->GetSession()->GetID());

		SendPacket.set_title(TCHAR_TO_UTF8(*title));

		auto SendBuffer = UJPacketHandler::MakeSendBuffer(SendPacket);
		JGameInst->SendAsync(SendBuffer);

		UE_LOG(LogTemp, Warning, TEXT("RoomEnterRequestSend"));
	}
}


bool UJLobbyUserWidget::OnRecvLobbyChat(UWorld* World, ProjectJ::S_LOBBY_CHAT& Packet, float Deltaseconds)
{
	UE_LOG(LogTemp, Log, TEXT("Lobby Chat %d %s %s"), Packet.account_id(), ANSI_TO_TCHAR(Packet.nickname().c_str()),
	       ANSI_TO_TCHAR(Packet.chat().c_str()));

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


bool UJLobbyUserWidget::OnRecvCreateRoom(UWorld* World, ProjectJ::S_LOBBY_CREATE_ROOM& Packet, float Deltaseconds)
{
	UE_LOG(LogTemp, Warning, TEXT("OnRecvCreateRoom"));

	auto JGameInst = Cast<UJGameInstance>(GetGameInstance());
	auto JGameMode = Cast<AJGameMode>(GetWorld()->GetAuthGameMode());

	if (Packet.result())
	{
		JGameMode->SetPlayerState(EGameState::Room);
		JGameMode->SetRoomId(Packet.room_id());
		JGameMode->SetSlotIndex(Packet.slot_index());

		if (AJPlayerController* PlayerController = Cast<AJPlayerController>(GetOwningPlayer()))
		{
			if (PlayerController->LobbyWidgetInstance)
			{
				PlayerController->LobbyWidgetInstance->RemoveFromParent();
			}

			PlayerController->RoomWidgetInstance = CreateWidget(PlayerController, PlayerController->RoomWidgetClass);
			if (PlayerController->RoomWidgetInstance)
			{
				PlayerController->RoomWidgetInstance->AddToViewport();
			}
		}
	}


	return true;
}
