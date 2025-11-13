// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/JRecvBuffer.h"

UJRecvBuffer::UJRecvBuffer()
{
	Capacity = BUFFER_SIZE * BUFFER_COUNT;
	Buffer.Init(0, Capacity);
}

void UJRecvBuffer::CleanupBuffer()
{
	const uint32 DataSize = GetDataSize();
	if (DataSize == 0)
	{
		WritePosition = ReadPosition = 0;
	}
	else
	{
		if (GetFreeSize() < BUFFER_SIZE)
		{
			FMemory::Memcpy(&Buffer[0], &Buffer.GetData()[ReadPosition], DataSize);
			ReadPosition = 0;
			WritePosition = DataSize;
		}
	}
}

bool UJRecvBuffer::WriteData(int32 Size)
{
	if (GetFreeSize() < Size)
	{
		return false;
	}

	WritePosition += Size;
	return true;
}

bool UJRecvBuffer::ReadData(int32 Size)
{
	if (GetDataSize() < Size)
	{
		return false;
	}

	ReadPosition += Size;
	return true;
}
