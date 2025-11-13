// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "UObject/NoExportTypes.h"
#include "JHttpObject.generated.h"

DECLARE_DELEGATE(FHttpResponseInvalid);
DECLARE_MULTICAST_DELEGATE_TwoParams(FHttpResponseOk, const FHttpRequestPtr&, const FHttpResponsePtr&);
DECLARE_DELEGATE_TwoParams(FHttpResponseCreated, const FHttpRequestPtr&, const FHttpResponsePtr&);
DECLARE_DELEGATE_TwoParams(FHttpResponseBadRequest, const FHttpRequestPtr&, const FHttpResponsePtr&);
DECLARE_DELEGATE_TwoParams(FHttpResponseDenied, const FHttpRequestPtr&, const FHttpResponsePtr&);
DECLARE_DELEGATE_TwoParams(FHttpResponseConflict, const FHttpRequestPtr&, const FHttpResponsePtr&);

// HTTP 통신 담당 클래스
// 최초 작성자: 박별
// 수정자: 
// 최종 수정일: 2023-10-06
UCLASS()
class PROJECTJ_API UJHttpObject : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE void Initialize(const FString& APIServerAddress) { Address = APIServerAddress; }

	// API 서버에 로그인 POST 수행 함수
	// Name: 계정 ID 문자열
	// Password: 계정 비밀번호 문자열
	void PostLoginRequest(const FString& Name, const FString& Password);

	// API 서버에 회원가입 POST 수행 함수
	// Name: 생성할 계정 ID 문자열
	// Nickname: 생성할 계정 닉네임 문자열
	// Password: 생성할 계정 비밀번호 문자열
	void PostCreateRequest(const FString& Name, const FString& Nickname, const FString& Password);

	// Delegates
public:
	FHttpResponseInvalid HttpResponseInvalidDelegate;
	FHttpResponseOk HttpResponseOkDelegate;
	FHttpResponseCreated HttpResponseCreatedDelegate;
	FHttpResponseBadRequest HttpResponseBadRequestDelegate;
	FHttpResponseDenied HttpResponseDeniedDelegate;
	FHttpResponseConflict HttpResponseConflictDelegate;

protected:
	// HTTP 응답이 도착했을 때 호출되는 콜백 함수
	// Request: HTTP 요청 객체
	// Response: HTTP 응답 객체
	// bConnectedSuccessfully: HTTP 요청 성공 여부
	void ProcessPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

protected:
	FString Address;
};
