// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Network/Protocol/Enum.pb.h"
#include "JRoomListItemData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTJ_API UJRoomListItemData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	int32 RoomID;

	UPROPERTY(BlueprintReadWrite)
	FString RoomTitle;

	UPROPERTY(BlueprintReadWrite)
	int32 RoomPlayerNum;

	UPROPERTY(BlueprintReadWrite)
	int32 RoomState;
};
