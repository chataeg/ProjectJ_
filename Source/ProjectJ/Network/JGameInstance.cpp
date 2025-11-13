// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/JGameInstance.h"
#include "Network/JPacketHandler.h"
#include "Player/JPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "UI/JLoginUserWidget.h"

UJGameInstance::UJGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UJGameInstance::Init()
{
	Super::Init();

	UJPacketHandler::Init();
	HttpObject = NewObject<UJHttpObject>();
	Socket = NewObject<UJSocket>();
	RecvThread = NewObject<UJRecvThread>();
	SendThread = NewObject<UJSendThread>();
	Session = NewObject<UJSession>();

	HttpObject->Initialize("127.0.0.1:8000");
	HttpOkDelegateHandle = HttpObject->HttpResponseOkDelegate.AddUObject(this, &UJGameInstance::ConnectSocketServer);
	TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateUObject(this, &UJGameInstance::Tick));
}

void UJGameInstance::Shutdown()
{
	Super::Shutdown();

	HttpObject->HttpResponseOkDelegate.Remove(HttpOkDelegateHandle);
	HttpObject->HttpResponseConflictDelegate.Unbind();
	FTSTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);

	Socket->Close();
	Session->Clear();

	StopThreads();
}

bool UJGameInstance::Tick(float DeltaSeconds)
{
	UJPacketHandler::ProcessPacket(GetWorld(), DeltaSeconds);

	return true;
}

void UJGameInstance::SendAsync(const TSharedPtr<FJSendBuffer>& SendBuffer)
{
	SendQueue.Enqueue(SendBuffer);
}

bool UJGameInstance::RunThreads()
{
	return RecvThread->RunThread(Socket, this) && SendThread->RunThread(Socket, this);
}

void UJGameInstance::StopThreads()
{
	RecvThread->StopThread();
	SendThread->StopThread();
}

void UJGameInstance::ConnectSocketServer(const FHttpRequestPtr& Request, const FHttpResponsePtr& Response)
{
	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(Reader, ResponseObj))
	{
		const FString Name = ResponseObj->GetStringField(TEXT("name"));
		const FString Nickname = ResponseObj->GetStringField(TEXT("nickname"));
		const FString Token = ResponseObj->GetStringField(TEXT("token"));
		const int32 Id = ResponseObj->GetNumberField(TEXT("player_id"));

		Session->SetSessionInfo(Id, Name, Nickname, Token);
		UE_LOG(LogTemp, Log, TEXT("PlayerIndex %d"), Id);
	}

	if (Socket->Init() == false)
	{
		Session->Clear();
		UE_LOG(LogTemp, Error, TEXT("Socket Init Error"));
		return;
	}

	if (Socket->Connect("127.0.0.1", 55141))
	{
		RecvThread->RunThread(Socket, this);
		SendThread->RunThread(Socket, this);

		ProjectJ::C_VERIFY_TOKEN Packet;
		Packet.set_account_id(Session->GetID());
		Packet.set_token(TCHAR_TO_UTF8(*Session->GetToken()));

		auto SendBuffer = UJPacketHandler::MakeSendBuffer(Packet);

		SendAsync(SendBuffer);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Socket Connect Error"));
	}
}
