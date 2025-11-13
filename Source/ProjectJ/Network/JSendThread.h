// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Network/JThread.h"
#include "JSendThread.generated.h"

class UJSocket;
class UJGameInstance;

// 소켓 수신 전용 스레드 클래스
// 최초 작성자: 박별
// 수정자: 
// 최종 수정일: 2023-10-10
UCLASS()
class PROJECTJ_API UJSendThread : public UJThread
{
	GENERATED_BODY()
public:
	virtual ~UJSendThread() override;

protected:
	virtual void WorkThread() override;
};
