// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Network/JHttpObject.h"
#include "Network/JSendBuffer.h"
#include "Network/JSocket.h"
#include "Network/JSession.h"
#include "Network/JRecvThread.h"
#include "Network/JSendThread.h"
#include "Network/Protocol/JPackets.h"
#include "JGameInstance.generated.h"

DECLARE_DELEGATE(FFailConnectSocketServer);

// 네트워크 관련 객체들을 관리하는 게임 인스턴스
// 최초 작성자: 박별
// 수정자: 차태관
// 최종 수정일: 2023-11-20
UCLASS()
class PROJECTJ_API UJGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UJGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void Shutdown() override;
	bool Tick(float DeltaSeconds);

	UJHttpObject* GetHttpObject() const { return HttpObject; }
	UJSocket* GetSocketObj() const { return Socket; }
	UJRecvThread* GetRecvThread() const { return RecvThread; }
	UJSendThread* GetSendThread() const { return SendThread; }
	TQueue<TSharedPtr<JPackets>>& GetPacketQueue() { return PacketQueue; }
	TQueue<TSharedPtr<FJSendBuffer>>& GetSendQueue() { return SendQueue; }
	UJSession* GetSession() const { return Session; }

public:
	// 소켓 서버로의 비동기 데이터 송신 함수
	// SendBuffer: 보낼 데이터의 스마트 포인터
	void SendAsync(const TSharedPtr<FJSendBuffer>& SendBuffer);

public:
	// Recv와 Send 스레드 가동 함수
	bool RunThreads();

	// Recv와 Send 스레드 종료 함수
	void StopThreads();

public:
	// 로그인 성공 이후 소켓 서버 연결 함수.
	// HTTP Ok 델리게이트에 바인드
	// Request: 델리게이트 선언 요구 인자로 HTTP 요청에 관련된 객체
	// Response: 델리게이트  선언 요구 인자로 HTTP 응답에 관련된 객체
	void ConnectSocketServer(const FHttpRequestPtr& Request, const FHttpResponsePtr& Response);

protected:
	UPROPERTY()
	UJHttpObject* HttpObject = nullptr;

	UPROPERTY()
	UJSocket* Socket = nullptr;

	UPROPERTY()
	UJRecvThread* RecvThread = nullptr;

	UPROPERTY()
	UJSendThread* SendThread = nullptr;

	UPROPERTY()
	UJSession* Session = nullptr;

	TQueue<TSharedPtr<JPackets>> PacketQueue;
	TQueue<TSharedPtr<FJSendBuffer>> SendQueue;
	FDelegateHandle HttpOkDelegateHandle;
	FTSTicker::FDelegateHandle TickDelegateHandle;
};
