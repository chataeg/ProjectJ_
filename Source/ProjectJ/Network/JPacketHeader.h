// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Network/JRecvBuffer.h"
#include "JPacketHeader.generated.h"

USTRUCT()
struct PROJECTJ_API FJPacketHeader
{
	GENERATED_BODY()

	uint16 Size = 0;
	uint16 Type = 0;
};