// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/JMoveable.h"

#include "Character/JPlayableCharacter.h"
#include "Component/JInventoryComponent.h"

AJMoveable::AJMoveable()
{
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = ItemMesh;

	ItemData.ItemID = -1;
	ItemData.ItemIndex = -1;
	ItemData.ItemClass = StaticClass();
	ItemData.ItemAddress = nullptr;
	ItemData.OwnerIndex = -1;
	ItemData.ItemImage = nullptr;
	ItemData.ItemImage_Rotated = nullptr;
	ItemData.IsInWorld = false;
	ItemData.WorldPosition = RemovedItemLocation;
	ItemData.WorldRotation = FRotator();
	ItemData.UIPosition = -1;
	ItemData.IsRotated = false;
	ItemData.UISize = FIntPoint();
	ItemData.Weight = 0;
}

void AJMoveable::MoveItemToLocation(FVector NewLocation, FRotator NewRotation)
{
	// 새 위치로 트랜스폼 수정
	AActor::SetActorLocation(NewLocation);
	UE_LOG(LogTemp,Warning,TEXT("%f %f %f"),GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z)
	// SetActorRotation(NewRotation.Quaternion());

}

void AJMoveable::RemoveItemFromLevel()
{
	MoveItemToLocation(RemovedItemLocation, FRotator());
}

void AJMoveable::InitItemData(const FItemData Input)
{
	ItemData.ItemID = Input.ItemID;
	ItemData.ItemIndex = Input.ItemIndex;
	ItemData.ItemClass = Input.ItemClass;
	ItemData.ItemAddress = this;
	ItemData.OwnerIndex = -1;
	ItemData.IsInWorld = Input.IsInWorld;
	if (ItemData.IsInWorld)
	{
		ItemData.WorldPosition = Input.WorldPosition;
		ItemData.WorldRotation = Input.WorldRotation;
		MoveItemToLocation(ItemData.WorldPosition, ItemData.WorldRotation);
		ItemData.UIPosition = -1;
	}
	else
	{
		ItemData.WorldPosition = RemovedItemLocation;
		ItemData.WorldRotation = FRotator(0,0,0);
		MoveItemToLocation(ItemData.WorldPosition, ItemData.WorldRotation);
		ItemData.UIPosition = Input.UIPosition;
	}
	ItemData.IsRotated = false;
	ItemData.UISize = Input.UISize;
	ItemData.Weight = Input.Weight;
}

void AJMoveable::ModifyItemData_UI(int OwnerID, int UIPosition, bool IsRotated)
{
	ItemData.OwnerIndex = OwnerID;
	ItemData.IsInWorld = false;
	ItemData.WorldPosition = RemovedItemLocation;
	ItemData.WorldRotation = FRotator();
	ItemData.UIPosition = UIPosition;
	ItemData.IsRotated = IsRotated;

	RemoveItemFromLevel();
}

void AJMoveable::ModifyItemData_World(FVector WorldPosition, FRotator WorldRotation)
{
	ItemData.OwnerIndex = -1;
	ItemData.IsInWorld = true;
	ItemData.WorldPosition = WorldPosition;
	ItemData.WorldRotation = WorldRotation;

	MoveItemToLocation(WorldPosition, WorldRotation);
}

void AJMoveable::Interact(AJPlayableCharacter* Character)
{
	UJInventoryComponent* InventoryComponent = Character->FindComponentByClass<UJInventoryComponent>();
	if (InventoryComponent)
	{
		InventoryComponent->RequestAddItem(this);
	}
}
