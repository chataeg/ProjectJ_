// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProjectJGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Network/JGameInstance.h"


AProjectJGameModeBase::AProjectJGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AProjectJGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AProjectJGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	//�ʿ�ÿ� �ʱ⼳��
	/*
	UGameUserSettings* setting = GEngine->GetGameUserSettings();

	if (setting)
	{
			
	}
	*/

	/*
	UJGameInstance* GameInstance = Cast<UJGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->GetHttpObject()->PostLoginRequest("test100", "test100");
	}

	UJPacketHandler::Packet_S_LOBBY_CHAT_Delegate.BindUObject(this, &AProjectJGameModeBase::TestRecv);

	GetWorldTimerManager().SetTimer(Handle, this, &AProjectJGameModeBase::TestSend, 1.0f, true);
		*/
}
/*
void AProjectJGameModeBase::TestSend()
{
	
	UJGameInstance* GameInstance = Cast<UJGameInstance>(GetGameInstance());

	ProjectJ::C_LOBBY_CHAT Chat;

	FString ChatString("Hi Server");
	Chat.set_chat(TCHAR_TO_UTF8(*ChatString));
	if (GameInstance)
	{
		Chat.set_nickname(TCHAR_TO_UTF8(*GameInstance->GetSession()->GetNickname()));
		Chat.set_account_id(GameInstance->GetSession()->GetId());
	}
	Chat.set_chat_time(0);

	auto SendBuffer = UJPacketHandler::MakeSendBuffer(Chat);

	if (GameInstance)
	{
		GameInstance->SendAsync(SendBuffer);
	}
	
}

bool AProjectJGameModeBase::TestRecv(UWorld* World, ProjectJ::S_LOBBY_CHAT& Packet)
{
	
	UE_LOG(LogTemp, Log, TEXT("%d %s %s"), Packet.account_id(), ANSI_TO_TCHAR(Packet.nickname().c_str()),
	       ANSI_TO_TCHAR(Packet.chat().c_str()));

	return true;
}
*/