// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JInventoryWidget.generated.h"

// 인벤토리 위젯 (WBP가 상속받음)
// 최초 작성자 : 김기홍
// 수정자 : 
// 최종 수정일 : 2023-10-20

UCLASS()
class PROJECTJ_API UJInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	UJInventoryComponent* InventoryComponent;
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	int TileSize;

public:
	// 인벤토리 위젯 초기화
	// _InventoryComponent : 해당 위젯을 생성한 컴포넌트
	// _TileSize : 한 칸(정사각형)의 크기(한 변의 길이)
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Inventory")
	void InitWidget(UJInventoryComponent* _InventoryComponent, int _TileSize);

	// 인벤토리 그리드의 위치 설정
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Inventory")
	void SetInventoryGridPosition(double x, double y);

	// 인벤토리 위젯 새로고침 (추가, 제거, Drag & Drop)
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Inventory")
	void RefreshInventory();

	// 인벤토리 배경 제거
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Inventory")
	void SetBackgroundTransparent(bool MakeTransparent);
};
