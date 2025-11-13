// Fill out your copyright notice in the Description page of Project Settings.


#include "FJCharacterManager.h"

#include "Character/JChaserCharacter.h"

FJCharacterManager::FJCharacterManager(UWorld* World)
{
	this->World = World;
	PlayerState = World->GetFirstPlayerController()->GetPlayerState<AJInGamePlayerState>();
}

FJCharacterManager::~FJCharacterManager()
{
}

void FJCharacterManager::InitCharacters(ProjectJ::S_MATCH_ALL_READY_TO_RECEIVE& Packet)
{
	int PlayerIndex = Packet.your_player_index();
	auto MatchInitInfo = Packet.info();

	auto ChaserInfo = MatchInitInfo.chaser();
	auto Fugitive_1_Info = MatchInitInfo.fugitive_first();
	auto Fugitive_2_Info = MatchInitInfo.fugitive_second();
	auto Fugitive_3_Info = MatchInitInfo.fugitive_third();

	auto Scale_1_Info = MatchInitInfo.scale_first();
	auto Scale_2_Info = MatchInitInfo.scale_second();
	auto Scale_3_Info = MatchInitInfo.scale_third();
	auto Scale_4_Info = MatchInitInfo.scale_fourth();

	// Chaser
	SpawnPlayer(AJChaserCharacter::StaticClass(), ChaserInfo);

	// Fugitive
	SpawnPlayer(AJChaserCharacter::StaticClass(), Fugitive_1_Info);
	SpawnPlayer(AJChaserCharacter::StaticClass(), Fugitive_2_Info);
	SpawnPlayer(AJChaserCharacter::StaticClass(), Fugitive_3_Info);

	// Scale
	UBlueprint* BlueprintClass = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), nullptr,
	                                                               TEXT("Blueprint'/Game/Blueprints/BP_JScale.BP_JScale'")));
	SpawnScale(BlueprintClass, Scale_1_Info);
	SpawnScale(BlueprintClass, Scale_2_Info);
	SpawnScale(BlueprintClass, Scale_3_Info);
	SpawnScale(BlueprintClass, Scale_4_Info);

	// Possess
	auto CharacterToControl = PlayerState->GetCharacter(PlayerIndex);
	UGameplayStatics::GetPlayerController(World, 0)->Possess(CharacterToControl);

	// InputMode
	FInputModeGameOnly InputModeGameOnly;
	UGameplayStatics::GetPlayerController(World, 0)->SetInputMode(InputModeGameOnly);
}

void FJCharacterManager::SpawnPlayer(UClass* Class, ProjectJ::PlayerInfo CharacterInfo)
{
	int alpha = 0;
	while (true)
	{
		FVector Correction = FVector(0, 0, alpha);
		AJCharacterBase* Character(
			CreateLocalObject(
				Class,
				GetFVector(CharacterInfo.position()) + Correction,
				GetFRotator(CharacterInfo.rotation()),
				CharacterInfo.player_index()));
		if (Character != nullptr)
		{
			PlayerState->AddCharacter(Character, CharacterInfo.player_index());
			UE_LOG(LogTemp, Warning, TEXT("Success To Spawn Player Character......"));
			return;
		}
		alpha++;
	}
}

void FJCharacterManager::SpawnScale(UBlueprint* BlueprintClass, ProjectJ::MatchInitInfo_ScaleInitInfo ScaleInfo)
{
	//AJCharacterBase* Scale(
	//    CreateLocalObject(
	//        BlueprintClass,
	//        GetFVector(ScaleInfo.position()),
	//        GetFRotator(ScaleInfo.rotation()),
	//        1));
	//if (Scale != nullptr)
	//{
	//    World->GetFirstPlayerController()->GetPlayerState<AJInGamePlayerState>()->AddCharacter(Scale, Scale.player_index());
	//}
}

AJCharacterBase* FJCharacterManager::CreateLocalObject
(UClass* Class, FVector Location, FRotator Rotator, int Index)
{
	if (World)
	{
		if (Class)
		{
			// Blueprint 클래스로 Actor 인스턴스를 생성합니다.
			AJCharacterBase* NewActor = World->SpawnActor<AJCharacterBase>(Class, Location, Rotator);
			if (NewActor)
			{
				NewActor->InitializeCharacter(Index);
				return NewActor;
			}
			UE_LOG(LogTemp, Warning, TEXT("Failed To Spawn Actor (FJCharacterManager)"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed To Load Blueprint (FJCharacterManager)"));
		}
	}
	return nullptr;
}

FVector FJCharacterManager::GetFVector(ProjectJ::Vector Vec)
{
	return FVector(Vec.x(), Vec.y(), Vec.z());
}

FRotator FJCharacterManager::GetFRotator(ProjectJ::Rotator Rot)
{
	return FRotator(Rot.roll(), Rot.pitch(), Rot.yaw());
}
