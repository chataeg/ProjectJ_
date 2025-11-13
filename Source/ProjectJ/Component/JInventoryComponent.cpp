// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/JInventoryComponent.h"
#include <Kismet/GameplayStatics.h>
#include "ProjectJ/Network/JGameInstance.h"
#include "FJCharacterManager.h"
#include "Player/JInGamePlayerState.h"


// Sets default values for this component's properties
UJInventoryComponent::UJInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	PayloadItemIndex = -1;

	// Create Widget
	CreateInventoryWidget();
}

UJInventoryComponent::~UJInventoryComponent()
{
	OwingItems.Empty();
}

// Called when the game starts
void UJInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	OwingItems = TMap<int, AJMoveable*>();
}

void UJInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	OwingItems.Empty();
}

void UJInventoryComponent::CreateInventoryWidget()
{
	static ConstructorHelpers::FClassFinder<UJInventoryWidget> WBP_Inventory
		(TEXT("/Game/Blueprints/Widget/WBP_Inventory"));
	if (!ensure(WBP_Inventory.Class != nullptr))
	{
		return;
	}

	if (WBP_Inventory.Succeeded())
	{
		InventoryWidgetClass = WBP_Inventory.Class;
	}
}

int UJInventoryComponent::IsInventorySpaceAvailable(FIntPoint Size)
{
	for (int i = 0; i < Row * Column; i++)
	{
		if (IsRoomAvailable(Size, i, false))
		{
			UE_LOG(LogTemp, Warning, TEXT("Find Room, Index = %d"), i);
			return i;
		}
	}

	for (int i = 0; i < Row * Column; i++)
	{
		if (IsRoomAvailable(Size, i, true))
		{
			UE_LOG(LogTemp, Warning, TEXT("Find Room, Index = %d"), i);
			return i;
		}
	}
	return -1;
}

bool UJInventoryComponent::IsRoomAvailable(FIntPoint Size, int TopLeftIndex, bool IsRotated)const
{
	FIntPoint Tile = IndexToTile(TopLeftIndex);
	if (IsRotated)
	{
		Size = FIntPoint(Size.Y, Size.X);
	}
	for (int i = 0; i < Size.X; i++)
	{
		for (int j = 0; j < Size.Y; j++)
		{
			if (!IsTileValid(Tile.X + i, Tile.Y + j))
			{
				return false;
			}
		}
	}
	return true;
}

bool UJInventoryComponent::IsOverWeight(int Weight)
{
	return MaxWeight <= GetCurWeight() + Weight;
}
bool UJInventoryComponent::IsTileValid(int X, int Y) const
{
	if (X < 0 || Y < 0 || X >= Column || Y >= Row) return false;
	const int Index = TileToIndex(FIntPoint(X, Y));
	if (InventorySlot[Index] != -1) return false;
	return true;
}

int UJInventoryComponent::TileToIndex(const FIntPoint Tile) const
{
	return Tile.X + Tile.Y * Column;
}

FIntPoint UJInventoryComponent::IndexToTile(const int Index) const
{
	return FIntPoint(Index % Column, Index / Column);
}

void UJInventoryComponent::InitializeInvnetoryComponent(int _Ownerindex, int _Row, int _Column, int _MaxWeight, double x, double y)
{
	if (!IsValid(InventoryWidgetClass))
	{
		return;
	}
	InventoryWidgetInstance = CreateWidget<UJInventoryWidget>(GetWorld(), InventoryWidgetClass);

	OwnerIndex = _Ownerindex;
	Owner = GetWorld()->GetFirstPlayerController()->GetPlayerState<AJInGamePlayerState>()->GetCharacter(OwnerIndex);
	UE_LOG(LogTemp, Warning, TEXT("Owner Index = %d"), OwnerIndex);

	Row = _Row;
	Column = _Column;
	MaxWeight = _MaxWeight;
	CurWeight = 0;

	if (InventoryWidgetInstance == nullptr)
	{
		return;
	}

	InventoryWidgetInstance->InitWidget(this, 50);
	InventoryWidgetInstance->SetInventoryGridPosition(x, y);

	InventorySlot = TArray<int>();
	InventorySlot.Init(-1, Row * Column);
}

void UJInventoryComponent::ToggleInventory()
{
	if (InventoryWidgetInstance == nullptr)
	{
		return;
	}
	if (IsDraging)
	{
		return;
	}
	if (InventoryWidgetInstance) 
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (InventoryWidgetInstance->IsInViewport())
		{
			InventoryWidgetInstance->RemoveFromParent();
			FInputModeGameOnly InputMode;
			PlayerController->SetInputMode(InputMode);
			PlayerController->bShowMouseCursor = false;
		}
		else
		{
			InventoryWidgetInstance->AddToViewport();
			FInputModeGameAndUI InputMode;
			PlayerController->SetInputMode(InputMode);
			PlayerController->bShowMouseCursor = true;
		}
	}
}

bool UJInventoryComponent::IsInventoryOpen()
{
	if (InventoryWidgetInstance)
		return InventoryWidgetInstance->IsInViewport();
	return false;
}

void UJInventoryComponent::SetBackgroundTransparent(bool MakeTransparent)
{
	if (InventoryWidgetInstance)
		InventoryWidgetInstance->SetBackgroundTransparent(MakeTransparent);
}

void UJInventoryComponent::RefreshInventoryWidget() const
{
	if(InventoryWidgetInstance)
		InventoryWidgetInstance->RefreshInventory();
}

void UJInventoryComponent::RequestAddItem(AJMoveable* Item)
{
	// Client Check 1 : 무게 확인
	if (IsOverWeight(Item->GetWeight()))
	{
		return;
	}

	// Client Check 2 : 공간 확인
	int TopLeftIndex = IsInventorySpaceAvailable(Item->GetUISize());
	if (TopLeftIndex != -1)
	{
		// 로컬 테스트용 코드
		/*
		if (IsRoomAvailable(Item->GetUISize(), TopLeftIndex, false))
		{
			ConductAddItemAt(Item, TopLeftIndex, false);
		}
		else if (IsRoomAvailable(Item->GetUISize(), TopLeftIndex, true))
		{
			ConductAddItemAt(Item, TopLeftIndex, true);
		}
		*/


		// 실사용 코드
		ProjectJ::C_MATCH_ITEM_PICKUP SendPacket;
		SendPacket.set_item_index(Item->GetItemIndex());
		SendPacket.set_player_index(OwnerIndex);
		auto temp = UJPacketHandler::MakeSendBuffer(SendPacket);
		UJGameInstance* GameInstance = Cast<UJGameInstance>(GetOwner()->GetGameInstance());
		if (GameInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("Add : Item Index = %d Owner Index = %d"), Item->GetItemIndex(), OwnerIndex);
			GameInstance->SendAsync(temp);
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("Failed to Add Item"));
	}
}

void UJInventoryComponent::RequestMoveItem(int FromIndex, AJMoveable* Item, int TopLeftIndex, bool IsRotated)
{
	// Client Check : 공간 확인
	if (IsRoomAvailable(Item->GetUISize(), TopLeftIndex, IsRotated))
	{
		// 로컬 테스트용 코드
		/*
		if (Owner->GetCharacterIndex() ==FromIndex)
		{
			ConductRelocateItemAt(Owner, Item, TopLeftIndex, IsRotated);
		}
		else
		{
			auto From = GetWorld()->GetFirstPlayerController()->GetPlayerState<AJInGamePlayerState>()->GetCharacter(FromIndex);
			ConductMoveItem(From, Owner, Item, TopLeftIndex, IsRotated);
		}
		*/
		
		// 실사용 코드
		ProjectJ::C_MATCH_ITEM_MOVE SendPacket;
		SendPacket.set_from_index(FromIndex);
		SendPacket.set_to_index(OwnerIndex);
		SendPacket.set_item_index(Item->GetItemIndex());
		SendPacket.set_is_item_rotated(Item->GetRotate());
		SendPacket.set_target_top_left_index(TopLeftIndex);

		UE_LOG(LogTemp, Warning, TEXT("Move : Item = %d Owner = %d"), Item->GetItemIndex(), FromIndex);

		auto temp = UJPacketHandler::MakeSendBuffer(SendPacket);
		UJGameInstance* GameInstance = Cast<UJGameInstance>(GetOwner()->GetGameInstance());
		if (GameInstance)
		{
			GameInstance->SendAsync(temp);
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("FFFF"));
	}
	//else
	//{
	//	// 로컬 테스트용 코드
	//	// 기존 위치로 롤백
	//	RevealItemAtInventorySlot(Item->GetItemIndex(), Item->GetUIPosition(), Item->GetUISize(), Item->GetRotate());
	//	return;
	//}
}

void UJInventoryComponent::RequestDropItem(AJMoveable* Item, FVector WorldPosition, FRotator WorldRotation)
{
	// 로컬 테스트용 코드
	/*
	ConductDropItemAt(Item, WorldPosition, WorldRotation);
	*/

	// 실사용 코드
	ProjectJ::C_MATCH_ITEM_DROP SendPacket;

	SendPacket.set_item_index(Item->GetItemIndex());
	SendPacket.set_player_index(OwnerIndex);

	auto WPosition = new ProjectJ::Vector();
	WPosition->set_x(WorldPosition.X);
	WPosition->set_y(WorldPosition.Y);
	WPosition->set_z(WorldPosition.Z);
	SendPacket.set_allocated_drop_item_position(WPosition);

	auto WRotator = new ProjectJ::Rotator();
	WRotator->set_pitch(WorldRotation.Pitch);
	WRotator->set_yaw(WorldRotation.Yaw);
	WRotator->set_roll(WorldRotation.Roll);
	SendPacket.set_allocated_drop_item_rotation(WRotator);

	auto temp = UJPacketHandler::MakeSendBuffer(SendPacket);
	UJGameInstance* GameInstance = Cast<UJGameInstance>(GetOwner()->GetGameInstance());
	if (GameInstance)
	{
		GameInstance->SendAsync(temp);
	}
}

void UJInventoryComponent::AddWeight(const int Weight)
{
	CurWeight += Weight;
}

void UJInventoryComponent::ConductAddItemAt(AJMoveable* Item, int TopLeftIndex, bool IsRotated)
{
	UE_LOG(LogTemp, Warning, TEXT("Add Item (Local)"));

	// Item 정보 변경 (Client)
	Item->ModifyItemData_UI(OwnerIndex, TopLeftIndex, IsRotated);

	// Inventory 정보 변경 (Client)
	OwingItems.Add(Item->GetItemIndex(), Item);
	AddWeight(Item->GetWeight());

	// Slot에 추가
	RevealItemAtInventorySlot(Item->GetItemIndex(), TopLeftIndex, Item->GetUISize(), IsRotated);
}

void UJInventoryComponent::ConductMoveItem(AJCharacterBase* From, AJCharacterBase* To, AJMoveable* Item, int TopLeftIndex, bool IsRotated)
{
	UE_LOG(LogTemp, Warning, TEXT("Move Item (Local) : %d -> %d"),From->CharacterIndex,To->CharacterIndex);
	auto FromInventoryComponent = From->InventoryComponent;
	auto ToInventoryComponent = To->InventoryComponent;

	// FROM
	FromInventoryComponent->OwingItems.Remove(Item->GetItemIndex());
	FromInventoryComponent->AddWeight(-Item->GetWeight());
	FromInventoryComponent->RefreshInventoryWidget();

	// TO
	ToInventoryComponent->ConductAddItemAt(Item, TopLeftIndex, IsRotated);
}

void UJInventoryComponent::ConductDropItemAt(AJMoveable* Item, FVector WorldPosition,
	FRotator WorldRotation)
{
	Item->ModifyItemData_World(WorldPosition, WorldRotation);

	OwingItems.Remove(Item->GetItemIndex());
	AddWeight(-Item->GetWeight());
}

void UJInventoryComponent::ConductRelocateItemAt(AJCharacterBase* Character, AJMoveable* Item, int TopLeftIndex, bool IsRotated)
{
	auto InventoryComponent = Character->InventoryComponent;

	// Item 정보 변경 (Client)
	Item->ModifyItemData_UI(OwnerIndex, TopLeftIndex, IsRotated);

	// Slot에 추가
	InventoryComponent->RevealItemAtInventorySlot(Item->GetItemIndex(), TopLeftIndex, Item->GetUISize(), IsRotated);
}

void UJInventoryComponent::RevealItemAtInventorySlot(int CharacterIndex, int ItemIndex, int TopLeftIndex, FIntPoint UISize, bool isRotated)
{
	auto Character = GetWorld()->GetFirstPlayerController()->GetPlayerState<AJInGamePlayerState>()->GetCharacter(CharacterIndex);
	Character->InventoryComponent->PayloadItemIndex = -1;
	Character->InventoryComponent->RevealItemAtInventorySlot(ItemIndex, TopLeftIndex, UISize, isRotated);
}

void UJInventoryComponent::HideItemAtInventorySlot(int ItemIndex)
{
	for (int i = 0; i < Row * Column; i++)
	{
		if (InventorySlot[i] == ItemIndex)
		{
			InventorySlot[i] = -1;
		}
	}
	RefreshInventoryWidget();
}

void UJInventoryComponent::RevealItemAtInventorySlot(int ItemIndex, int TopLeftIndex, FIntPoint UISize, bool isRotated)
{
	FIntPoint Tile = IndexToTile(TopLeftIndex);
	if (isRotated == false)
	{
		for (int i = 0; i < UISize.X; i++)
		{
			for (int j = 0; j < UISize.Y; j++)
			{
				InventorySlot[TileToIndex(FIntPoint(Tile.X + i, Tile.Y + j))] = ItemIndex;
			}
		}
	}
	else
	{
		UISize = FIntPoint(UISize.Y, UISize.X);
		for (int i = 0; i < UISize.X; i++)
		{
			for (int j = 0; j < UISize.Y; j++)
			{
				InventorySlot[TileToIndex(FIntPoint(Tile.X + i, Tile.Y + j))] = ItemIndex;
			}
		}
	}
	RefreshInventoryWidget();
}
