// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/JSendBuffer.h"
#include "Kismet/GameplayStatics.h"
#include "JGameInstanceSubsystem.h"

TQueue<FJSendBuffer*> UJSendBufferManager::SendBufferPool;
TSharedPtr<FJSendBufferBlock> UJSendBufferManager::BackBufferBlock = nullptr;
TSharedPtr<FJSendBufferBlock> UJSendBufferManager::FrontBufferBlock = nullptr;

FJSendBuffer::FJSendBuffer()
{
}

FJSendBuffer::FJSendBuffer(uint8* BufferPtr, uint32 AllocSize, const TSharedPtr<FJSendBufferBlock>& OwnerPtr)
	: Buffer(BufferPtr), AllocSize(AllocSize), Owner(OwnerPtr)
{
}

FJSendBuffer::~FJSendBuffer()
{
}

void FJSendBuffer::Close(uint32 WrittenSize)
{
	check(WriteSize <= AllocSize);
	WriteSize = WrittenSize;
	Owner->Close(WrittenSize);
}

FJSendBufferBlock::FJSendBufferBlock()
{
	Buffer.Init(0, SEND_BUFFER_BLOCK_SIZE);
}

FJSendBufferBlock::~FJSendBufferBlock()
{
}

void FJSendBufferBlock::Reset()
{
	bIsOpen = false;
	UsedSize = 0;
}

TSharedPtr<FJSendBuffer> FJSendBufferBlock::Open(uint32 AllocSize)
{
	check(AllocSize <= SEND_BUFFER_BLOCK_SIZE);
	check(bIsOpen == false);

	if (AllocSize > GetFreeSize())
	{
		return nullptr;
	}

	bIsOpen = true;

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GameViewport->GetWorld());
	if (GameInstance)
	{
		UJGameInstanceSubsystem* SendBufferSubsystem = GameInstance->GetSubsystem<UJGameInstanceSubsystem>();

		if (SendBufferSubsystem)
		{
			FJSendBuffer* SendBuffer = UJSendBufferManager::Pop(AllocSize);
			new(SendBuffer)FJSendBuffer(GetBuffer(), AllocSize, SharedThis(this));
			return MakeShareable(SendBuffer, &UJSendBufferManager::Push);
		}
	}

	return nullptr;
}

void FJSendBufferBlock::Close(uint32 WrittenSize)
{
	check(bIsOpen == true);
	bIsOpen = false;
	UsedSize += WrittenSize;
}

UJSendBufferManager::UJSendBufferManager()
{
	FrontBufferBlock = MakeShared<FJSendBufferBlock>();
	BackBufferBlock = MakeShared<FJSendBufferBlock>();

	FrontBufferBlock->Reset();
	BackBufferBlock->Reset();
}


TSharedPtr<FJSendBuffer> UJSendBufferManager::Open(uint32 Size)
{
	check(FrontBufferBlock->IsOpen() == false);

	if (FrontBufferBlock->GetFreeSize() < Size)
	{
		Swap(FrontBufferBlock, BackBufferBlock);
		FrontBufferBlock->Reset();
	}

	return FrontBufferBlock->Open(Size);
}

FJSendBuffer* UJSendBufferManager::Pop(uint32 AllocSize)
{
	FJSendBuffer* SendBuffer = nullptr;

	if (SendBufferPool.Dequeue(SendBuffer) == false)
	{
		SendBuffer = static_cast<FJSendBuffer*>(FMemory::Malloc(sizeof(FJSendBuffer)));
	}

	return SendBuffer;
}

void UJSendBufferManager::Push(FJSendBuffer* SendBuffer)
{
	SendBufferPool.Enqueue(SendBuffer);
}
