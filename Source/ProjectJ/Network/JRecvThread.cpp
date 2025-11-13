// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/JRecvThread.h"
#include "Network/JSocket.h"
#include "Network/JGameInstance.h"
#include "Network/JPacketHandler.h"
#include "Network/JPacketHeader.h"

UJRecvThread::~UJRecvThread()
{
}

bool UJRecvThread::Init()
{
	RecvBuffer = NewObject<UJRecvBuffer>();
	return UJThread::Init();
}

void UJRecvThread::WorkThread()
{
	FPlatformProcess::Sleep(0.03f);

	while (StopTaskCounter.GetValue() == 0 && Socket->IsConnected())
	{
		FPlatformProcess::Sleep(0.001f);
		int32 RecvSize = 0;
		if (Socket->IsConnected() && Socket->GetSocket()->Recv(
			RecvBuffer->GetWriteBufferPos(), RecvBuffer->GetFreeSize(), RecvSize))
		{
			if (RecvSize == 0)
			{
				Socket->HandleError("Recv 0");
				break;
			}

			if (RecvBuffer->WriteData(RecvSize) == false)
			{
				Socket->HandleError("Write Error");
				break;
			}

			int32 DataSize = RecvBuffer->GetDataSize();
			int32 ProcessSize = ProcessRecv(RecvBuffer->GetReadBufferPos(), DataSize);
			if (ProcessSize < 0 || DataSize < ProcessSize || RecvBuffer->ReadData(ProcessSize) == false)
			{
				Socket->HandleError("Read Error");
				break;
			}

			RecvBuffer->CleanupBuffer();
		}
		else
		{
			Socket->HandleError("Fail to Socket Receive");
			break;
		}
	}
}


int32 UJRecvThread::ProcessRecv(uint8* Buffer, int32 Size)
{
	int32 ProcessSize = 0;

	while (true)
	{
		int32 DataSize = Size - ProcessSize;

		if (DataSize < sizeof(FJPacketHeader))
		{
			break;
		}

		FJPacketHeader* Header = reinterpret_cast<FJPacketHeader*>(&Buffer[ProcessSize]);
		if (DataSize < Header->Size)
		{
			break;
		}

		TSharedPtr<JPackets> PacketPtr = UJPacketHandler::HandlePacket(&Buffer[ProcessSize], Header->Size);
		if (PacketPtr != nullptr)
		{
			GameInstance->GetPacketQueue().Enqueue(PacketPtr);
		}

		ProcessSize += Header->Size;
	}

	return ProcessSize;
}
