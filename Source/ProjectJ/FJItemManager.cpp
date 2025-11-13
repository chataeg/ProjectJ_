// Fill out your copyright notice in the Description page of Project Settings.


#include "FJItemManager.h"
#include "ProjectJ/Pawn/JMoveable.h"
#include "Engine/World.h"
#include "Player/JInGamePlayerState.h"

FJItemManager::FJItemManager(UWorld* World)
{
	this->World = World;
	PlayerState = World->GetFirstPlayerController()->GetPlayerState<AJInGamePlayerState>();

}

void FJItemManager::GenerateItems(ProjectJ::S_MATCH_ITEM_GENERATED& Packet)
{
	auto OriginalData = Packet.items();
	TMap<int, FItemData> Failed = TMap<int, FItemData>();
	for (auto Item : OriginalData)
	{
		FItemData Input = FItemData();
		Input.ItemID = Item.id();
		Input.ItemIndex = Item.index();
		UBlueprint* BlueprintClass = GetBluprintClass(Input.ItemID);
		Input.ItemClass = BlueprintClass->ParentClass;
		Input.OwnerIndex = Item.onwer_player_index();
		Input.IsInWorld = !Item.is_owned();
		Input.WorldPosition = GetFVector(Item.world_position());
		Input.WorldRotation = GetFRotator(Item.world_rotation());
		Input.UISize = FIntPoint(Item.width(), Item.height());
		Input.IsRotated = Item.is_rotated();
		Input.Weight = Item.weight();

		AJMoveable* Object(CreateLocalObject(BlueprintClass, Input));
		if (Object != nullptr)
		{
			PlayerState->AddItem(Object, Input.ItemIndex);
			UE_LOG(LogTemp, Warning, TEXT("Success To Spawn Item......"));
		}
		// 생성을 실패한 경우
		else
		{
			Failed.Add(Input.ItemID, Input);
		}
	}

	// CreateLocalObject를 실패한 오브젝트에 대한 후처리
	TArray<int> Keys;
	int alpha = 0;
	do
	{
		Failed.GetKeys(Keys);
		for (auto Key : Keys)
		{
			FItemData Data;
			Failed.RemoveAndCopyValue(Key, Data);
			Data.WorldPosition = FVector(Data.WorldPosition.X, Data.WorldPosition.Y, Data.WorldPosition.Z + alpha);
			UBlueprint* BlueprintClass = GetBluprintClass(Data.ItemID);
			AJMoveable* Object = CreateLocalObject(BlueprintClass, Data);
			if (Object != nullptr)
			{
				PlayerState->AddItem(Object, Data.ItemIndex);
				UE_LOG(LogTemp, Warning, TEXT("Success To Re-Spawn Item......"));
			}
			else
			{
				Failed.Add(Key, Data);
			}
		}
		alpha++;
	} while (Keys.Num() > 0);
}

FVector FJItemManager::GetFVector(ProjectJ::Vector vec)
{
	return FVector(vec.x(), vec.y(), vec.z());
}

FRotator FJItemManager::GetFRotator(ProjectJ::Rotator rot)
{
	return FRotator(rot.roll(), rot.pitch(), rot.yaw());
}

UBlueprint* FJItemManager::GetBluprintClass(int itemID)
{
	UBlueprint* Ret = nullptr;
	switch (itemID)
	{
	case 0:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Garbage/BP_GarbageCan_1.BP_GarbageCan_1'")
		));
		break;
	case 1:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Garbage/BP_GarbageCan_2.BP_GarbageCan_2'")
		));
		break;
	case 2:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Garbage/BP_GarbageCan_3.BP_GarbageCan_3'")
		));
		break;
	case 3:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Garbage/BP_GarbageCan_4.BP_GarbageCan_4'")
		));
		break;
	case 4:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Garbage/BP_GarbageCan_5.BP_GarbageCan_5'")
		));
		break;
	case 5:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Garbage/BP_GarbageCardBox_1.BP_GarbageCardBox_1'")
		));
	case 6:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Garbage/BP_GarbageCardBox_2.BP_GarbageCardBox_2'")
		));
		break;
	case 7:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Garbage/BP_GarbageCardBox_3.BP_GarbageCardBox_3'")
		));
		break;
	case 8:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Garbage/BP_GarbageCardBox_4.BP_GarbageCardBox_4'")
		));
		break;
	case 9:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Garbage/BP_GarbageCardBox_5.BP_GarbageCardBox_5'")
		));
		break;
	case 10:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Garbage/BP_GarbageChip_1.BP_GarbageChip_1'")
		));
		break;
	case 11:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Garbage/BP_GarbageChip_2.BP_GarbageChip_2'")
		));
		break;
	case 12:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Garbage/BP_GarbageCoffeeCup.BP_GarbageCoffeeCup'")
		));
		break;
	case 13:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Garbage/BP_GarbageMilk.BP_GarbageMilk'")
		));
		break;
	case 14:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Garbage/BP_GarbagePaperBag.BP_GarbagePaperBag'")
		));
		break;
	case 15:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Garbage/BP_GarbageSmallbox_1.BP_GarbageSmallbox_1'")
		));
		break;
	case 16:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Garbage/BP_GarbageSmallbox_2.BP_GarbageSmallbox_2'")
		));
		break;
	case 17:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Garbage/BP_GarbageSmallbox_3.BP_GarbageSmallbox_3'")
		));
		break;
	case 18:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Garbage/BP_GarbageSmallbox_4.BP_GarbageSmallbox_4'")
		));
		break;
	case 19:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Garbage/BP_GarbageTrashBag_1.BP_GarbageTrashBag_1'")
		));
		break;
	case 20:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Garbage/BP_GarbageTrashBag_2.BP_GarbageTrashBag_2'")
		));
		break;
	case 21:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Garbage/BP_GarbageTrashBag_3.BP_GarbageTrashBag_3'")
		));
		break;
	case 22:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Grocery/BP_GroceryBeer.BP_GroceryBeer'")
		));
		break;
	case 23:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Grocery/BP_GroceryEnergyCan.BP_GroceryEnergyCan'")
		));
		break;
	case 24:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Grocery/BP_GroceryFlour.BP_GroceryFlour'")
		));
		break;
	case 25:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Grocery/BP_GroceryJam.BP_GroceryJam'")
		));
		break;
	case 26:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Grocery/BP_GroceryOil.BP_GroceryOil'")
		));
		break;
	case 27:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Grocery/BP_GroceryPotato.BP_GroceryPotato'")
		));
		break;
	case 28:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Grocery/BP_GrocerySodaCan.BP_GrocerySodaCan'")
		));
		break;
	case 29:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Grocery/BP_GroceryWater.BP_GroceryWater'")
		));
		break;
	case 30:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Grocery/BP_GroceryWine.BP_GroceryWine'")
		));
		break;
	case 31:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Papers/BP_PapersBook_1.BP_PapersBook_1'")
		));
		break;
	case 32:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Papers/BP_PapersBook_2.BP_PapersBook_2'")
		));
		break;
	case 33:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Papers/BP_PapersBook_3.BP_PapersBook_3'")
		));
		break;
	case 34:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Papers/BP_PapersBook_4.BP_PapersBook_4'")
		));
		break;
	case 35:
		Ret = Cast<UBlueprint>(StaticLoadObject
		(UBlueprint::StaticClass(), nullptr,
			TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Moveable/Papers/BP_PapersBook_5.BP_PapersBook_5'")
		));
		break;
	}
	return Ret;
}

AJMoveable* FJItemManager::CreateLocalObject(UBlueprint* BlueprintClass, FItemData ItemData)
{
	if (World)
	{
		if (BlueprintClass)
		{
			// Blueprint 클래스로 Actor 인스턴스를 생성합니다.
			AJMoveable* NewActor = World->SpawnActor<AJMoveable>(BlueprintClass->GeneratedClass, ItemData.WorldPosition, ItemData.WorldRotation);
			if (NewActor)
			{
				NewActor->InitItemData(ItemData);
				return NewActor;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed To Spawn Actor (JFItemManager)"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed To Load Blueprint (JFItemManager)"));
		}
	}
	return nullptr;
}
