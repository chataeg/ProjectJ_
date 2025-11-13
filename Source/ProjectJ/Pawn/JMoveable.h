// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawn/JPawnBase.h"
#include "../ProjectJ/Structs.h"
#include "Character/JCharacterBase.h"
#include "Interface/JInteractableInterface.h"
#include "JMoveable.generated.h"

// 플레이어가 이동시킬 수 있는 (인벤토리에 넣기, 들기 등등) Pawn
// 해당 클래스를 상속받는 오브젝트 : "아이템"
// 최초 작성자 : 김기홍
// 수정자 : 
// 최종 수정일 : 2023-11-13

UCLASS()
class PROJECTJ_API AJMoveable : public AJPawnBase, public IJInteractableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AJMoveable();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "Item")
	class UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemData ItemData;

	// 월드에서 제거된 아이템들이 존재하는 위치
	const FVector RemovedItemLocation = FVector(0,5000,0);

	// 월드의 아이템 위치 변경
	void MoveItemToLocation(FVector NewLocation, FRotator NewRotation);

public:
	// ItemData 초기화
	void InitItemData(const FItemData ItemData);

	// ItemData 수정
	void ModifyItemData_UI(int OwnerID, int UIPosition, bool IsRotated);
	void ModifyItemData_World(FVector WorldPosition, FRotator WorldRotation);

	// Getter
	// 아이템 각각의 고유번호를 나타낸다
	const int GetItemIndex() { return ItemData.ItemIndex; }
	const int GetOwnerID() { return ItemData.OwnerIndex; }
	const bool GetRotate() { return ItemData.IsRotated; }
	const FIntPoint GetUISize() { return ItemData.UISize; }
	const int GetUIPosition() { return ItemData.UIPosition; }
	const int GetWeight() { return ItemData.Weight; }

	// Setter
	void SetRotate(bool IsRotated) { ItemData.IsRotated = IsRotated; }

	// 월드에서 아이템을 제거
	void RemoveItemFromLevel();

	// UJInteractableInterface interface
	virtual void Interact(AJPlayableCharacter* Character) override;
	virtual void TerminateInteract(AJPlayableCharacter* Character) override{};
	// End of UJInteractableInterface interface
};
