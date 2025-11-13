// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Structs.h"
#include "FJItemManager.h"
#include "Widget/JInventoryWidget.h"
#include "ProjectJ/Pawn/JMoveable.h"

#include "Character/JCharacterBase.h"
#include "Components/ActorComponent.h"
#include "ProjectJ/Network/JPacketHandler.h"
#include "ProjectJ/Network/JGameInstance.h"
#include "JInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangedEvent);

class UJInventoryWidget;

// 인벤토리 컴포넌트, AJPlayableCharacter에 부착하여 사용함
// 아이템의 원본 데이터에 접근하여 변경
// 최초 작성자 : 김기홍
// 수정자 : 
// 최종 수정일 : 2023-11-13


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTJ_API UJInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Owner
	AJCharacterBase* Owner;
	int OwnerIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	int PayloadItemIndex;

	// Sets default values for this component's properties
	UJInventoryComponent();
	virtual ~UJInventoryComponent() override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Inventory
	TArray<int> InventorySlot;
	TMap<int, AJMoveable*> OwingItems;
	int Row, Column;
	int CurWeight, MaxWeight;
	bool IsDraging;

	// Inventory Widget
	TSubclassOf<class UUserWidget> InventoryWidgetClass;
	class UJInventoryWidget* InventoryWidgetInstance;

protected:
	virtual void CreateInventoryWidget();

	///////////////////////////////////////////////////
	/// Client Check
	// TopLeftIndex위치에 ItemData를 넣을수 있는지를 반환
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int IsInventorySpaceAvailable(FIntPoint Size);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsRoomAvailable(FIntPoint Size, int TopLeftIndex, bool IsRotated)const;
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsOverWeight(int Weight);

	///////////////////////////////////////////////////
	/// Tile
	// Index Transform
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int TileToIndex(FIntPoint Tile) const;
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FIntPoint IndexToTile(int Index) const;
	// (_X,_Y)좌표가 인벤토리UI에서 유효한 좌표인지를 반환
	bool IsTileValid(int _X, int _Y) const;

public:
	///////////////////////////////////////////////////
	/// Initialize
	virtual void InitializeInvnetoryComponent(int _Ownerindex, int _Row, int _Column,int _MaxWeight, double x, double y);

	///////////////////////////////////////////////////
	/// Inventory Control
	void ToggleInventory();
	bool IsInventoryOpen();
	void SetBackgroundTransparent(bool MakeTransparent);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RefreshInventoryWidget() const;

	///////////////////////////////////////////////////
	/// Request To Server
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RequestAddItem(AJMoveable* Item);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RequestMoveItem(int FromIndex, AJMoveable* Item, int TopLeftIndex, bool IsRotated);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RequestDropItem(AJMoveable* Item, FVector WorldPosition, FRotator WorldRotation);

	///////////////////////////////////////////////////
	/// Getter
	UFUNCTION(BlueprintPure, Category = "Inventory")
	int GetRow() { return Row; }

	UFUNCTION(BlueprintPure, Category = "Inventory")
	int GetColumn() { return Column; }

	UFUNCTION(BlueprintPure, Category = "Inventory")
	int GetMaxWeight() { return MaxWeight; }

	UFUNCTION(BlueprintPure, Category = "Inventory")
	int GetCurWeight() { return CurWeight; }

	bool GetDraging() { return IsDraging; }
	UFUNCTION(BlueprintPure, Category = "Inventory")
	TMap<int, AJMoveable*> GetOwingItems() { return OwingItems; }

	///////////////////////////////////////////////////
	/// Setter
	virtual void AddWeight(const int Weight);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetDraging(bool _IsDraging) { IsDraging = _IsDraging; }

	///////////////////////////////////////////////////
	/// Conduct
	void ConductAddItemAt(AJMoveable*, int TopLeftIndex, bool IsRotated);
	void ConductMoveItem(AJCharacterBase*, AJCharacterBase*, AJMoveable*, int TopLeftIndex, bool IsRotated);
	void ConductDropItemAt(AJMoveable*, FVector WorldPosition, FRotator WorldRotation);
	// Only used in Client
	void ConductRelocateItemAt(AJCharacterBase*, AJMoveable*, int TopLeftIndex, bool IsRotated);
	
	///////////////////////////////////////////////////
	/// Show & Hide Item In Inventory Slot
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RevealItemAtInventorySlot(int CharacterIndex, int ItemIndex, int TopLeftIndex, FIntPoint UISize, bool isRotated);
	void RevealItemAtInventorySlot(int ItemIndex, int TopLeftIndex, FIntPoint UISize, bool isRotated);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void HideItemAtInventorySlot(int ItemIndex);
};
