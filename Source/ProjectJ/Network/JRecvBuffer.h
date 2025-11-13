// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JRecvBuffer.generated.h"

// 소켓 수신 버퍼 클래스
// 최초 작성자: 박별
// 수정자: 
// 최종 수정일: 2023-11-24
UCLASS()
class PROJECTJ_API UJRecvBuffer : public UObject
{
	GENERATED_BODY()
	enum
	{
		BUFFER_COUNT = 10,
		BUFFER_SIZE = 0x4000
	};

public:
	UJRecvBuffer();

public:
	FORCEINLINE int32 GetDataSize() const { return WritePosition - ReadPosition; }
	FORCEINLINE int32 GetFreeSize() const { return Capacity - WritePosition; }
	FORCEINLINE uint8* GetWriteBufferPos() { return &Buffer.GetData()[WritePosition]; }
	FORCEINLINE uint8* GetReadBufferPos() { return &Buffer.GetData()[ReadPosition]; }

public:
	// 커서 정리 함수
	void CleanupBuffer();

	// 데이터를 수신받은 이후 쓰기 커서를 이동시키는 함수
	// Size: 수신받은 데이터의 크기
	// 커서 이동 성공 여부
	bool WriteData(int32 Size);

	// 데이터를 읽은 이후 읽기 커서를 이동시키는 함수
	// Size: 읽은 데이터의 크기
	// 커서 이동 성공 여부
	bool ReadData(int32 Size);

protected:
	int32 WritePosition = 0;
	int32 ReadPosition = 0;
	int32 Capacity = 0;
	TArray<uint8> Buffer;
};
