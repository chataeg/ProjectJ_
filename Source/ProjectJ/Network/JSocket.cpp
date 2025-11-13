// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/JSocket.h"

#include "JGameInstance.h"

UJSocket::~UJSocket()
{
	Close();
}

bool UJSocket::Init()
{
	if (Socket != nullptr)
	{
		delete Socket;
	}

	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("FSocket"));
	Socket->SetNoDelay();

	return Socket != nullptr;
}

bool UJSocket::Connect(const FString& IPString, uint16 Port)
{
	if (Socket != nullptr)
	{
		FIPv4Address IpAddress;
		FIPv4Address::Parse(IPString, IpAddress);

		FIPv4Endpoint Endpoint(IpAddress, Port);

		return Socket->Connect(*Endpoint.ToInternetAddr());
	}

	return false;
}

void UJSocket::HandleError(const FString& Cause)
{
	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	ESocketErrors ErrorCode = SocketSubsystem->GetLastErrorCode();
	FString ErrorCodeString = SocketSubsystem->GetSocketError(ErrorCode);

	UE_LOG(LogTemp, Error, TEXT("Socket HandleError Cause: %s %s"), *Cause, *ErrorCodeString);
	Close();

	switch (ErrorCode)
	{
	case SE_ECONNRESET:
	case SE_ECONNABORTED:
		if (ServerClosedDelegate.IsBound())
		{
			ServerClosedDelegate.Execute();
		}
		break;
	default:
		break;
	}
}

void UJSocket::Close()
{
	if (Socket != nullptr)
	{
		Socket->Close();
		delete Socket;
		Socket = nullptr;
	}
}
