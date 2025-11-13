// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Networking.h"
#include "JSocket.generated.h"

DECLARE_DELEGATE(FServerClosed);

class UJGameInstance;

// 클라이언트 소켓 관련 클래스
// 최초 작성자: 박별
// 수정자: 
// 최종 수정일: 2023-10-06
UCLASS()
class PROJECTJ_API UJSocket : public UObject
{
	GENERATED_BODY()
public:
	virtual ~UJSocket() override;

public:
	FORCEINLINE FSocket* GetSocket() const { return Socket; }
	FORCEINLINE APlayerController* GetPlayerController() const { return PlayerController; }
	FORCEINLINE void SetPlayerController(APlayerController* InPlayerController) { PlayerController = InPlayerController; }
	FORCEINLINE void SetGameInstance(UJGameInstance* InGameInstance) { GameInstance = InGameInstance; }

public:
	FORCEINLINE bool IsConnected() { return Socket && Socket->GetConnectionState() == SCS_Connected; }

	// 네트워크 관련
public:
	// 소켓 초기화 및 설정 함수
	// 소켓 생성 성공 여부
	bool Init();

	// 소켓 서버 연결 함수
	// IPString: 소켓 서버 IP 주소 문자열
	// Port: 소켓 서버 포트 번호
	// 소켓 서버와 연결 성공 여부
	bool Connect(const FString& IPString, uint16 Port);

	// 소켓 에러 핸들링 함수
	// Cause: 에러 발생 사유 문자열
	void HandleError(const FString& Cause);

	// 소켓 연결 종료 함수
	void Close();

public:
	FServerClosed ServerClosedDelegate;

protected:
	UPROPERTY()
	APlayerController* PlayerController = nullptr;

	UPROPERTY()
	UJGameInstance* GameInstance = nullptr;

	FSocket* Socket = nullptr;
};
