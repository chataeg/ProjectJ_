// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Network/JThread.h"
#include "UObject/NoExportTypes.h"
#include "Network/JRecvBuffer.h"
#include "JRecvThread.generated.h"

class UJSocket;
class UJGameInstance;

// 소켓 수신 전용 스레드 클래스
// 최초 작성자: 박별
// 수정자: 
// 최종 수정일: 2023-10-10
UCLASS()
class PROJECTJ_API UJRecvThread : public UJThread
{
	GENERATED_BODY()

public:
	virtual ~UJRecvThread() override;

public:
	virtual bool Init() override;
	FORCEINLINE UJRecvBuffer* GetRecvBuffer() const { return RecvBuffer; }

	// 수신 받은 데이터를 처리하는 함수
	// Buffer: 처리할 데이터가 존재하는 버퍼의 주소
	// Size: 수신 받은 데이터의 크기
	// 처리한 데이터의 크기를 반환
	int32 ProcessRecv(uint8* Buffer, int32 Size);

protected:
	virtual void WorkThread() override;

protected:
	UPROPERTY()
	UJRecvBuffer* RecvBuffer = nullptr;
};
