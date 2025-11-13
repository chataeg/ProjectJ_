// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JThread.generated.h"

class UJSocket;
class UJGameInstance;

// 네트워크 통신 스레드 부모 클래스
// 최초 작성자: 박별
// 수정자: 
// 최종 수정일: 2023-10-10
UCLASS()
class PROJECTJ_API UJThread : public UObject, public FRunnable
{
	GENERATED_BODY()
public:
	UJThread();
	virtual ~UJThread() override;

public:
	virtual bool Init() override;
	virtual uint32 Run() override sealed;
	virtual void Stop() override sealed;
	virtual void Exit() override sealed;

	// 스레드 가동 함수
	// InSocket: 현재 게임의 UJSocket 객체 포인터
	// InGameInstance: 현재 게임의 GameInstance 객체 포인터
	// 스레드 가동에 성공했는지 여부를 반환
	bool RunThread(UJSocket* InSocket, UJGameInstance* InGameInstance);

	// 스레드 종료 함수
	void StopThread();

protected:
	// 스레드에서 작업할 동작 함수
	// 상속한 클래스에서 구현
	virtual void WorkThread()
	{
	};

protected:
	UPROPERTY()
	UJSocket* Socket = nullptr;

	UPROPERTY()
	UJGameInstance* GameInstance = nullptr;

	FRunnableThread* Thread = nullptr;

	FThreadSafeCounter StopTaskCounter = 0;
};
