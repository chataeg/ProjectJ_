// Fill out your copyright notice in the Description page of Project Settings.


#include "JInGameMode.h"
#include "JGameMode.h"
#include "Engine/LevelStreaming.h"
#include "Network/JGameInstance.h"
#include "FJCharacterManager.h"
#include "Character/JScale.h"
#include "Component/JInventoryComponent.h"
#include "Component/JScaleInventoryComponent.h"
#include "Player/JInGamePlayerState.h"

AJInGameMode::AJInGameMode()
{
	UE_LOG(LogTemp, Warning, TEXT("JInGameMode Constructor"));
	IsItemGenerated = false;
	IsCharacterGenerated = false;

	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	/// Set Game Mode
	DefaultPawnClass = nullptr;
	PlayerStateClass = AJInGamePlayerState::StaticClass();

	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	/// Bind Packet

	///////////////////////////////////////////////////
	/// Generate - Player
	UJPacketHandler::Packet_S_MATCH_ALL_READY_TO_RECEIVE_Delegate.BindUObject(this, &AJInGameMode::OnRecvPlayerInfo);

	///////////////////////////////////////////////////
	/// Generate - Item
	UJPacketHandler::Packet_S_MATCH_ITEM_GENERATED_Delegate.BindUObject(this, &AJInGameMode::OnRecvGenerateItems);

	///////////////////////////////////////////////////
	/// Match - Start
	UJPacketHandler::Packet_S_MATCH_START_Delegate.BindUObject(this, &AJInGameMode::OnRecvMatchStart);

	///////////////////////////////////////////////////
	/// Match - End


	///////////////////////////////////////////////////
	/// Response - Inventory
	UJPacketHandler::Packet_S_MATCH_ITEM_SOMEONE_PICKUP_Delegate.BindUObject(this, &AJInGameMode::OnRecvAddItem);
	UJPacketHandler::Packet_S_MATCH_ITEM_SOMEONE_MOVE_Delegate.BindUObject(this, &AJInGameMode::OnRecvMoveItem);
	UJPacketHandler::Packet_S_MATCH_ITEM_SOMEONE_DROP_Delegate.BindUObject(this, &AJInGameMode::OnRecvDropItem);

	///////////////////////////////////////////////////
	/// Response - Scale
	UJPacketHandler::Packet_S_MATCH_SCALE_ON_CHANGED_Delegate.BindUObject(this, &AJInGameMode::OnRecvScaleOnChanged);

	///////////////////////////////////////////////////
	/// Response - Player
	UJPacketHandler::Packet_S_MATCH_CHASER_ATTACK_Delegate.BindUObject(this, &AJInGameMode::OnRecvChaserAttack);
	UJPacketHandler::Packet_S_MATCH_CHASER_HIT_Delegate.BindUObject(this, &AJInGameMode::OnRecvChaserHit);

	static ConstructorHelpers::FClassFinder<UUserWidget> InGameWidget(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/BP_InGame.BP_InGame_C'"));
	if (InGameWidget.Succeeded())
	{
		InGameWidgetClass = InGameWidget.Class;
	}

	UE_LOG(LogTemp, Warning, TEXT("LevelLoaded"));
	OnRequestReady2Recv();
	UE_LOG(LogTemp, Warning, TEXT("OnRequestReady2Recv"));
}


void AJInGameMode::OnRequestReady2Recv()
{
	ProjectJ::C_MATCH_READY_TO_RECEIVE SendPacket;

	//UE_LOG(LogTemp, Warning, TEXT("GameInstance : %p"), UGameplayStatics::GetGameInstance(GetWorld()));


	if (auto JGameInst = Cast<UJGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		SendPacket.set_account_id(JGameInst->GetSession()->GetID());

		auto SendBuffer = UJPacketHandler::MakeSendBuffer(SendPacket);
		JGameInst->SendAsync(SendBuffer);


		UE_LOG(LogTemp, Warning, TEXT("Ingame : Ready2Recv RequestSend"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FAIL = Ingame : Ready2Recv RequestSend"));
	}
}

void AJInGameMode::OnRequestMatchStart()
{
	if (auto JGameInst = Cast<UJGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		UE_LOG(LogTemp, Warning, TEXT("Ingame : On Request Match Start"));

		ProjectJ::C_MATCH_READY_TO_START SendPacket;
		SendPacket.set_player_index(YourPlayerIndex);
		UE_LOG(LogTemp, Warning, TEXT("%d"), YourPlayerIndex);
		auto SendBuffer = UJPacketHandler::MakeSendBuffer(SendPacket);
		JGameInst->SendAsync(SendBuffer);
	}
}

bool AJInGameMode::OnRecvMatchStart(UWorld* World, ProjectJ::S_MATCH_START& Packet, float Deltaseconds)
{
	//캐릭터 스폰과 아이템 스폰이 모두 끝나 OnRequestMatchStart 를 호출 후 돌려받는 패킷입니다.
	UE_LOG(LogTemp, Warning, TEXT("Ingame : OnRecvMatchStart"));

	auto JGameInst = Cast<UJGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	InGameWidgetInstance = Cast<UJInGameUserWidget>(CreateWidget(JGameInst, InGameWidgetClass));
	if (InGameWidgetInstance)
	{
		//Cast<AJInGamePlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0))->
		InGameWidgetInstance->EndTick = Packet.end_tick();
		InGameWidgetInstance->CurrentTick = Packet.current_tick();
	}


	return true;
}

bool AJInGameMode::OnRecvPlayerInfo(UWorld* World, ProjectJ::S_MATCH_ALL_READY_TO_RECEIVE& Packet, float Deltaseconds)
{
	UE_LOG(LogTemp, Warning, TEXT("Ingame : OnRecvPlayerInfo"));

	auto JGameInst = Cast<UJGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	CharacterManager = MakeShared<FJCharacterManager>(GetWorld());
	CharacterManager->InitCharacters(Packet);
	UE_LOG(LogTemp, Warning, TEXT("Character init fin"));


	//JGameInst->InGameWidgetInstance->AddToViewport();
	UE_LOG(LogTemp, Warning, TEXT("InGame UI Added to Viewport"));

	//Cast<UJInGameUserWidget>(JGameInst->InGameWidgetInstance)->Init(Packet);

	UE_LOG(LogTemp, Warning, TEXT("InGame UI init fin"));


	//UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(GetWorld()->GetFirstPlayerController()->GetPlayerState<AJInGamePlayerState>()->GetCharacter(0));

	IsCharacterGenerated = true;
	YourPlayerIndex = Packet.your_player_index();
	if (IsItemGenerated && IsCharacterGenerated)
	{
		OnRequestMatchStart();
	}

	return true;
}

bool AJInGameMode::OnRecvGenerateItems(UWorld* World, ProjectJ::S_MATCH_ITEM_GENERATED& Packet, float Deltaseconds)
{
	ItemManager = MakeShared<FJItemManager>(GetWorld());
	ItemManager->GenerateItems(Packet);
	UE_LOG(LogTemp, Warning, TEXT("Ingame : OnRecvGenerateItems"));

	IsItemGenerated = true;
	if (IsItemGenerated && IsCharacterGenerated)
	{
		OnRequestMatchStart();
	}


	return true;
}

bool AJInGameMode::OnRecvAddItem(UWorld* World, ProjectJ::S_MATCH_ITEM_SOMEONE_PICKUP& Packet, float Deltaseconds)
{
	UE_LOG(LogTemp, Warning, TEXT("Add Item (Server)"));

	auto PlayerIndex = Packet.player_index();
	auto ItemIndex = Packet.item_index();
	auto TopLeftIndex = Packet.top_left_index();
	auto IsItemRotated = Packet.is_item_rotated();

	auto Item = GetWorld()->GetFirstPlayerController()->GetPlayerState<AJInGamePlayerState>()->GetItem(ItemIndex);
	auto Character = GetWorld()->GetFirstPlayerController()->GetPlayerState<AJInGamePlayerState>()->GetCharacter(PlayerIndex);


	UE_LOG(LogTemp, Warning, TEXT("Item = %d Owner = %d"), ItemIndex, PlayerIndex);

	Character->InventoryComponent->ConductAddItemAt(Item, TopLeftIndex, IsItemRotated);

	return true;
}

bool AJInGameMode::OnRecvMoveItem(UWorld* World, ProjectJ::S_MATCH_ITEM_SOMEONE_MOVE& Packet, float Deltaseconds)
{
	auto PlayerIndex = Packet.player_index();
	auto FromIndex = Packet.from_index();
	auto ToIndex = Packet.to_index();
	auto ItemIndex = Packet.item_index();
	auto TopLeftIndex = Packet.top_left_index();
	auto IsItemRotated = Packet.is_item_rotated();

	UE_LOG(LogTemp, Warning, TEXT("Move Item (Server) : %d -> %d"), FromIndex, ToIndex);

	auto Character = GetWorld()->GetFirstPlayerController()->GetPlayerState<AJInGamePlayerState>()->GetCharacter(PlayerIndex);

	auto From = GetWorld()->GetFirstPlayerController()->GetPlayerState<AJInGamePlayerState>()->GetCharacter(FromIndex);
	auto To = GetWorld()->GetFirstPlayerController()->GetPlayerState<AJInGamePlayerState>()->GetCharacter(ToIndex);

	auto Item = GetWorld()->GetFirstPlayerController()->GetPlayerState<AJInGamePlayerState>()->GetItem(ItemIndex);

	Character->InventoryComponent->ConductMoveItem(From, To, Item, TopLeftIndex, IsItemRotated);
	return true;
}

bool AJInGameMode::OnRecvDropItem(UWorld* World, ProjectJ::S_MATCH_ITEM_SOMEONE_DROP& Packet, float Deltaseconds)
{
	auto PlayerIndex = Packet.player_index();
	auto ItemIndex = Packet.item_index();
	auto _DropPosition = Packet.drop_item_position();
	auto _DropRotation = Packet.drop_item_rotation();

	auto Character = GetWorld()->GetFirstPlayerController()->GetPlayerState<AJInGamePlayerState>()->GetCharacter(PlayerIndex);
	auto Item = GetWorld()->GetFirstPlayerController()->GetPlayerState<AJInGamePlayerState>()->GetItem(ItemIndex);
	auto DropPosition = FVector(_DropPosition.x(), _DropPosition.y(), _DropPosition.z());
	auto DropRotation = FRotator(_DropRotation.roll(), _DropRotation.pitch(), _DropRotation.yaw());

	Character->InventoryComponent->ConductDropItemAt(Item, DropPosition, DropRotation);
	return true;
}

bool AJInGameMode::OnRecvScaleOnChanged(UWorld* World, ProjectJ::S_MATCH_SCALE_ON_CHANGED& Packet, float Deltaseconds)
{
	auto Character = GetWorld()->GetFirstPlayerController()->GetPlayerState<AJInGamePlayerState>()->GetCharacter(Packet.scale_index());
	auto Scale = Cast<AJScale>(Character);
	auto Inventory = Scale->InventoryComponent;
	auto ScaleInventory = Cast<UJScaleInventoryComponent>(Inventory);

	if (Packet.is_operating())
	{
		return false;
	}
	if (ScaleInventory->GetMaxWeight() >= Packet.current_weight() && ScaleInventory->GetMinWeight() <= Packet.current_weight())
	{
		Scale->OpenDoor();
	}
	return true;
}

bool AJInGameMode::OnRecvChaserAttack(UWorld* World, ProjectJ::S_MATCH_CHASER_ATTACK& Packet, float Deltaseconds)
{
	return true;
}

bool AJInGameMode::OnRecvChaserHit(UWorld* World, ProjectJ::S_MATCH_CHASER_HIT& Packet, float Deltaseconds)
{
	return true;
}

/*
UJGameInstance* AJInGameMode::GetJGameInstance()
{
	return JGameInst;
}

void AJInGameMode::SetJGameInstance(UJGameInstance* input)
{
	JGameInst = input;
}


*/
