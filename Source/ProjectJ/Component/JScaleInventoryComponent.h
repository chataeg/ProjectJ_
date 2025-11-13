// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/JInventoryComponent.h"
#include "JScaleInventoryComponent.generated.h"

// AJScale에 부착되는 인벤토리 컴포넌트
// 최초 작성자: 김기홍
// 수정자: 
// 최종 수정일: 2023-11-19
UCLASS()
class PROJECTJ_API UJScaleInventoryComponent : public UJInventoryComponent
{
	GENERATED_BODY()
public:
	UJScaleInventoryComponent();
protected:
	// Inventory
	int MinWeight;

protected:
	virtual void CreateInventoryWidget() override;

public:
	///////////////////////////////////////////////////
	/// Initialize
	void InitializeInvnetoryComponent(int _OwnerIndex, 
		int _Row, int _Column,
		int _MinWeight, int _MaxWeight,
		double x, double y);

	///////////////////////////////////////////////////
	/// Getter
	UFUNCTION(BlueprintPure, Category = "Inventory")
	int GetMinWeight() { return MinWeight; }
};
