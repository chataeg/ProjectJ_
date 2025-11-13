// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/JSendThread.h"
#include "Network/JSocket.h"
#include "Network/JGameInstance.h"
#include "Network/JPacketHandler.h"

UJSendThread::~UJSendThread()
{
}

void UJSendThread::WorkThread()
{
	FPlatformProcess::Sleep(0.03f);

	while (StopTaskCounter.GetValue() == 0 && Socket->IsConnected())
	{
		FPlatformProcess::Sleep(0.001f);
		TSharedPtr<FJSendBuffer> SendBuffer;
		if (GameInstance->GetSendQueue().Dequeue(SendBuffer))
		{
			int32 SendSize = 0;
			if (Socket->IsConnected() && Socket->GetSocket()->Send(
				SendBuffer->GetBuffer(), SendBuffer->GetWriteSize(), SendSize) == false)
			{
				Socket->HandleError("Fail to Socket Send");

				break;
			}
		}
	}
}
