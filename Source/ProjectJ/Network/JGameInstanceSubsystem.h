// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Network/JSendBuffer.h"
#include "JGameInstanceSubsystem.generated.h"

// 송신 버퍼 매니저 서브시스템 클래스
// 최초 작성자: 박별
// 수정자: 
// 최종 수정일: 2023-10-06
UCLASS()
class PROJECTJ_API UJGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	UPROPERTY()
	UJSendBufferManager* SendBufferManager;
};
