// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JSession.generated.h"

// 클라이언트의 세션 정보를 관리하는 클래스
// 최초 작성자: 박별
// 수정자: 
// 최종 수정일: 2023-10-06
UCLASS()
class PROJECTJ_API UJSession : public UObject
{
	GENERATED_BODY()

public:
	void SetSessionInfo(const int32& InId, const FString& InName, const FString& InNickname, const FString& InToken);
	FORCEINLINE FString& GetName() { return Name; }
	FORCEINLINE FString& GetNickname() { return Nickname; }
	FORCEINLINE FString& GetToken() { return Token; }
	FORCEINLINE int32 GetID() { return SessionId; }

public:
	// 클라이언트 세션의 정보를 초기화 하는 함수
	void Clear();

protected:
	int32 SessionId = -1;
	FString Name;
	FString Nickname;
	FString Token;
};
