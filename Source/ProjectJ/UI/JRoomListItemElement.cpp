// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/JRoomListItemElement.h"
#include "UI/JRoomListItemData.h"

void UJRoomListItemElement::NativeConstruct()
{
	Super::NativeConstruct();

	IDTextBlock = Cast<UTextBlock>(GetWidgetFromName("RoomIDText"));
	check(IDTextBlock);

	TitleTextBlock = Cast<UTextBlock>(GetWidgetFromName("RoomTitleText"));
	check(TitleTextBlock);

	PlayerNumTextBlock = Cast<UTextBlock>(GetWidgetFromName("RoomPlayerNumText"));
	check(PlayerNumTextBlock);

	StateTextBlock = Cast<UTextBlock>(GetWidgetFromName("RoomStateText"));
	check(StateTextBlock);
}

void UJRoomListItemElement::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	UJRoomListItemData* Data = Cast<UJRoomListItemData>(ListItemObject);
	if (Data)
	{
		IDTextBlock->SetText(FText::AsNumber(Data->RoomID));
		TitleTextBlock->SetText(FText::FromString(Data->RoomTitle));
		PlayerNumTextBlock->SetText(FText::FromString(FString::FromInt(Data->RoomPlayerNum) + " / 4"));
		StateTextBlock->SetText(FText::FromString(Data->RoomState == ProjectJ::WAITING ? TEXT("대기중") : TEXT("게임중")));
		RoomID = Data->RoomID;
		StateCode = static_cast<int32>(Data->RoomState);
	}
}
