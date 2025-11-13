// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/JScale.h"
#include "Character/JPlayableCharacter.h"
#include "Component/JScaleInventoryComponent.h"
#include "Components/AudioComponent.h"

AJScale::AJScale()
{
	// StaticMesh Component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	// Create Inventory Component
	InventoryComponent = CreateDefaultSubobject<UJScaleInventoryComponent>(TEXT("InventoryComponent"));

	// Create Audio Component
	// UAudioComponent 생성
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent); // 원하는 컴포넌트에 Attach할 수 있음

	// 사운드 파일 로드
	static ConstructorHelpers::FObjectFinder<USoundWave> MovingSoundAsset
	(TEXT("/Script/Engine.SoundWave'/Game/Blueprints/Earth_Rumble.Earth_Rumble'"));

	if (MovingSoundAsset.Succeeded())
		MovingSound = MovingSoundAsset.Object;
	
	// UAudioComponent에 사운드 지정
	AudioComponent->SetSound(MovingSound);
}

void AJScale::BeginPlay()
{
	Super::BeginPlay();
	// Initialize Variables
	IsOpen = 0;
	LastInteractCharacter = nullptr;
	StartingLocation = GetActorLocation();

	AudioComponent->Stop();
}

void AJScale::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(IsOpen == 1)
	{
		auto CurLocation = GetActorLocation();
		SetActorLocation(FVector(CurLocation.X, CurLocation.Y, CurLocation.Z - MoveSpeed * DeltaSeconds));
		
		if(FVector::Dist(StartingLocation,GetActorLocation())>MoveDistance)
		{
			UE_LOG(LogTemp, Warning, TEXT("Stop Sound"));
			IsOpen = 2;
			if (AudioComponent)
			{
				AudioComponent->Stop();
			}
		}
	}
}

void AJScale::ToggleInventory()
{
	if (InventoryComponent)
	{
		InventoryComponent->ToggleInventory();
	}
}

bool AJScale::IsInventoryOpen()
{
	if (InventoryComponent) {
		return InventoryComponent->IsInventoryOpen();
	}
	return false;
}

void AJScale::OpenDoor()
{
	UE_LOG(LogTemp, Warning, TEXT("Start Sound"));
	IsOpen = 1;
	TerminateInteract(LastInteractCharacter);
	if (AudioComponent)
	{
		AudioComponent->Play();
	}
}


void AJScale::InitializeCharacter(int _CharacterIndex)
{
	CharacterIndex = _CharacterIndex;
	// Initialize Inventory
	Cast<UJScaleInventoryComponent>(InventoryComponent)->InitializeInvnetoryComponent
	(
		CharacterIndex,
		15, 6,
		20, 30,
		600, 0);
}

void AJScale::Interact(AJPlayableCharacter* Character)
{
	if (IsOpen) {
		if (LastInteractCharacter) 
		{
			TerminateInteract(LastInteractCharacter);
		}
		return;
	}

	if (IsInventoryOpen() == false)
	{
		ToggleInventory();
	}
	if (Character->InventoryComponent->IsInventoryOpen() == false)
	{
		Character->InventoryComponent->ToggleInventory();
		Character->InventoryComponent->SetBackgroundTransparent(true);
	}
	LastInteractCharacter = Character;
}

void AJScale::TerminateInteract(AJPlayableCharacter* Character)
{
	if(Character->InventoryComponent->IsInventoryOpen())
	{
		Character->InventoryComponent->ToggleInventory();
	}
	if (IsInventoryOpen())
	{
		ToggleInventory();
	}
	if (Character->InventoryComponent->IsInventoryOpen())
	{
		Character->InventoryComponent->ToggleInventory();
	}
	Character->InventoryComponent->SetBackgroundTransparent(false);
	LastInteractCharacter = nullptr;
}
