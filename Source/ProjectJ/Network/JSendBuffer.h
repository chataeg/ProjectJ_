// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JSendBuffer.generated.h"

class FJSendBufferBlock;
class UJSendBufferManager;

// 소켓 송신 버퍼 클래스
// 최초 작성자: 박별
// 수정자: 
// 최종 수정일: 2023-10-06
class PROJECTJ_API FJSendBuffer
{
public:
	FJSendBuffer();
	FJSendBuffer(uint8* BufferPtr, uint32 AllocSize, const TSharedPtr<FJSendBufferBlock>& OwnerPtr);
	~FJSendBuffer();

public:
	FORCEINLINE uint8* GetBuffer() { return Buffer; }
	FORCEINLINE uint32 GetAllocSize() { return AllocSize; }
	FORCEINLINE uint32 GetWriteSize() { return WriteSize; }

	// 송신 버퍼에 쓰기를 종료하는 함수
	// WrittenSize: 사용한 메모리 크기
	void Close(uint32 WrittenSize);

protected:
	uint8* Buffer;
	uint32 AllocSize = 0;
	uint32 WriteSize = 0;
	TSharedPtr<FJSendBufferBlock> Owner;
};

// 소켓 송신 버퍼 메모리 풀 클래스
// 최초 작성자: 박별
// 수정자: 
// 최종 수정일: 2023-10-06
class PROJECTJ_API FJSendBufferBlock : public TSharedFromThis<FJSendBufferBlock>
{
	enum
	{
		SEND_BUFFER_BLOCK_SIZE = 0x4000
	};

public:
	FJSendBufferBlock();
	~FJSendBufferBlock();

public:
	bool IsOpen() { return bIsOpen; }
	uint8* GetBuffer() { return &Buffer[UsedSize]; }
	uint32 GetFreeSize() { return SEND_BUFFER_BLOCK_SIZE - UsedSize; }

public:
	// 블록 상태 초기화 함수
	void Reset();

	// 메모리 풀에서 일정 크기만큼 메모리를 사용한는 함수
	// AllocSize: 사용할 메모리의 크기
	// Buffer에서 AllocSize만큼 사용 가능한 FJSendBuffer의 스마트 포인터 반환
	// FJSedBuffer 스마트 포인터 생성 실패시 nullptr 반환
	TSharedPtr<FJSendBuffer> Open(uint32 AllocSize);

	// 메모리에 데이터 기록을 종료하는 함수
	// WrittenSize: 사용한 메모리 크기
	void Close(uint32 WrittenSize);

protected:
	TArray<uint8> Buffer;
	bool bIsOpen = false;
	uint32 UsedSize = 0;
};


// 소켓 송신 버퍼 매니저 클래스
// 최초 작성자: 박별
// 수정자: 
// 최종 수정일: 2023-10-06
UCLASS()
class PROJECTJ_API UJSendBufferManager : public UObject
{
	GENERATED_BODY()

public:
	UJSendBufferManager();

public:
	// 송신 버퍼를 요청하는 함수
	// Size: 송신할 데이터의 크기
	// FrontBufferBlock에서 Size 만큼 사용 가능한 FJSendBuffer의 스마트 포인터 반환
	// FJSedBuffer 스마트 포인터 생성 실패시 nullptr 반환
	TSharedPtr<FJSendBuffer> Open(uint32 Size);

	// FJSendBuffer 풀에서 객체를 하나 꺼내는 함수
	// AllocSize: 사용할 메모리 크기
	// 생성자가 호출되어 초기화된 FJSendBuffer 객체 주소
	static FJSendBuffer* Pop(uint32 AllocSize);

	// FJSendBuffer 스마트 포인터 Deleter 함수
	// SendBuffer: 참조가 끝난 SendBuffer 객체 주소
	static void Push(FJSendBuffer* SendBuffer);

protected:
	static TQueue<FJSendBuffer*> SendBufferPool;
	static TSharedPtr<FJSendBufferBlock> BackBufferBlock;
	static TSharedPtr<FJSendBufferBlock> FrontBufferBlock;
};
