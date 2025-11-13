// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Network/JGameInstance.h"
#include "Network/JHttpObject.h"
#include "Interfaces/IHttpRequest.h"
#include "Network/JSendBuffer.h"
#include "Network/JSocket.h"
#include "Network/JSession.h"
#include "Network/JRecvThread.h"
#include "Network/JSendThread.h"
#include "Network/Protocol/JPackets.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "JLoginUserWidget.generated.h"

// 로그인 위젯 구현부
// 최초 작성자 : 차태관
// 수정자 : 박별
// 최종 수정일 : 2023-11-20
UCLASS()
class PROJECTJ_API UJLoginUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UJLoginUserWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// 로그인 요청 
	UFUNCTION(BlueprintCallable)
	void LoginRequested(const FString& Name, const FString& Password);

	void LoginResponseOk(const FHttpRequestPtr& Request, const FHttpResponsePtr& Response);
	void LoginResponseConflict(const FHttpRequestPtr& Request, const FHttpResponsePtr& Response);
	void LoginResponseUnauthorized(const FHttpRequestPtr& Request, const FHttpResponsePtr& Response);


	void HttpResponseInvalid();


	FString ParseDetail(const FHttpResponsePtr& Response);

	// 회원가입 요청 
	UFUNCTION(BlueprintCallable)
	void SignUpRequested(const FString& Name, const FString& Password, const FString& Nickname);

	void SignUpResponseCreated(const FHttpRequestPtr& Request, const FHttpResponsePtr& Response);
	void SignUpResponseBadRequest(const FHttpRequestPtr& Request, const FHttpResponsePtr& Response);
	void SignUpResponseConflict(const FHttpRequestPtr& Request, const FHttpResponsePtr& Response);


	bool LoginVerified(UWorld* World, ProjectJ::S_VERIFY_TOKEN& Packet, float DeltaSeconds);

	FString GetDetail();
	void SetDetail(FString input);

	void PopMessage(FText Message);


protected:
	UPROPERTY()
	UJGameInstance* JGameInst;

	UPROPERTY(BlueprintReadWrite)
	FString Detail;

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
	FDelegateHandle DelegateHandle;
};
