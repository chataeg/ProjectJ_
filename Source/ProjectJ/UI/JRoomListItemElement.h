// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "JRoomListItemElement.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJ_API UJRoomListItemElement : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

public:
	UPROPERTY(BlueprintReadWrite)
	int32 RoomID;

	UPROPERTY(BlueprintReadWrite)
	int32 StateCode;

protected:
	UPROPERTY()
	TObjectPtr<UTextBlock> IDTextBlock;

	UPROPERTY()
	TObjectPtr<UTextBlock> TitleTextBlock;

	UPROPERTY()
	TObjectPtr<UTextBlock> PlayerNumTextBlock;

	UPROPERTY()
	TObjectPtr<UTextBlock> StateTextBlock;
};
