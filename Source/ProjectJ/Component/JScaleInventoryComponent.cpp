// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/JScaleInventoryComponent.h"

#include "Player/JInGamePlayerState.h"

UJScaleInventoryComponent::UJScaleInventoryComponent()
{
	CreateInventoryWidget();
}

void UJScaleInventoryComponent::CreateInventoryWidget()
{
	static ConstructorHelpers::FClassFinder<UJInventoryWidget> WBP_Inventory
	(TEXT("/Game/Blueprints/Widget/WBP_ScaleInventory"));
	if (!ensure(WBP_Inventory.Class != nullptr)) return;

	if (WBP_Inventory.Succeeded()) {
		InventoryWidgetClass = WBP_Inventory.Class;
	}
}

void UJScaleInventoryComponent::InitializeInvnetoryComponent
(
	int _OwnerIndex, int _Row, int _Column,
	int _MinWeight, int _MaxWeight,
	double x, double y)
{
	if (!IsValid(InventoryWidgetClass)) return;
	InventoryWidgetInstance = CreateWidget<UJInventoryWidget>(GetWorld(), InventoryWidgetClass);

	OwnerIndex = _OwnerIndex;
	Owner = GetWorld()->GetFirstPlayerController()->GetPlayerState<AJInGamePlayerState>()->GetCharacter(OwnerIndex);

	Row = _Row; Column = _Column;
	MinWeight = _MinWeight; MaxWeight = _MaxWeight; CurWeight = 0;

	if (InventoryWidgetInstance == nullptr) return;

	InventoryWidgetInstance->InitWidget(this, 50);
	InventoryWidgetInstance->SetInventoryGridPosition(x, y);

	InventorySlot = TArray<int>();
	InventorySlot.Init(-1, Row * Column);
}
