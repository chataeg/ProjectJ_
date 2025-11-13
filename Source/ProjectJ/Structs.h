// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs.generated.h"

class AJCharacterBase;

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

	// 아이템의 종류를 나타낸다
	UPROPERTY(BlueprintReadOnly)
	int ItemID;

	// 아이템 각각의 고유번호를 나타낸다
	UPROPERTY(BlueprintReadOnly)
	int ItemIndex;

	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<class AJMoveable> ItemClass;

	UPROPERTY(BlueprintReadOnly)
	AJMoveable* ItemAddress;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMaterialInterface* ItemImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMaterialInterface* ItemImage_Rotated;

	UPROPERTY(BlueprintReadOnly)
	int OwnerIndex;

	UPROPERTY(BlueprintReadOnly)
	bool IsInWorld;

	UPROPERTY(BlueprintReadOnly)
	FVector WorldPosition;

	UPROPERTY(BlueprintReadOnly)
	FRotator WorldRotation;

	UPROPERTY(BlueprintReadOnly)
	int UIPosition;

	UPROPERTY(BlueprintReadOnly)
	bool IsRotated;

	UPROPERTY(BlueprintReadOnly)
	FIntPoint UISize;

	UPROPERTY(BlueprintReadOnly)
	int Weight;
};
