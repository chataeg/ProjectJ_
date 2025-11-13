// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/JPlayableCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectJ/Interface/JInteractableInterface.h"
#include "Player/JPlayerController.h"

AJPlayableCharacter::AJPlayableCharacter()
{
	
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	
	// GetCharacterMovement()->bOrientRotationToMovement = true;
	// GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 200.f;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(RootComponent);

	// Create InventoryComponent
	InventoryComponent = CreateDefaultSubobject<UJInventoryComponent>(TEXT("InventoryComponent"));

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DEFAULT_CONTEXT(TEXT("/Game/Blueprints/Input/IMC_Default.IMC_Default"));
	if (DEFAULT_CONTEXT.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Find IMC_Default"));
		DefaultMappingContext = DEFAULT_CONTEXT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Move(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/Actions/IA_Move.IA_Move'"));
	if (IA_Move.Object)
	{
		MoveAction = IA_Move.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Look(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/Actions/IA_Look.IA_Look'"));
	if (IA_Look.Object)
	{
		LookAction = IA_Look.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_ToggleInventory
	(TEXT("/Game/Blueprints/Input/Actions/IA_ToggleInventory.IA_ToggleInventory"));
	if (IA_ToggleInventory.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Find IA_ToggleInventory"));
		ToggleInventoryAction = IA_ToggleInventory.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Interact
	(TEXT("/Game/Blueprints/Input/Actions/IA_Interact.IA_Interact"));
	if (IA_Interact.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Find IA_Interact"));
		InteractAction = IA_Interact.Object;
	}

}

void AJPlayableCharacter::InitializeCharacter(int _CharacterIndex)
{
	CharacterIndex = _CharacterIndex;

	// Initialize Inventory
	InventoryComponent->InitializeInvnetoryComponent(CharacterIndex, 15, 6, 1500, -600, 0);
}

void AJPlayableCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Add Input Mapping Context
	if (AJPlayerController* PlayerController = Cast<AJPlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Add Input Mapping Context"));
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Register Event - Outline Trace
	// 어딘가에서 타이머 설정
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AJPlayableCharacter::TraceForOutline, 0.1f, true);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AJPlayableCharacter::SendMyInfoToServer, 0.016f, true);

}

//////////////////////////////////////////////////////////////////////////// Input
void AJPlayableCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	//// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AJPlayableCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AJPlayableCharacter::Look);

		//ToggleInventory
		EnhancedInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &AJPlayableCharacter::ToggleInventory);

		//Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AJPlayableCharacter::Interact);
	}
}


UCameraComponent* AJPlayableCharacter::GetCameraComponent()
{
	return FirstPersonCameraComponent;
}


void AJPlayableCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AJPlayableCharacter::Look(const FInputActionValue& Value)
{

	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AJPlayableCharacter::ToggleInventory()
{
	if (IsInteractingWithNPC) return;
	if (InventoryComponent)
	{
		InventoryComponent->ToggleInventory();
	}

}

bool AJPlayableCharacter::IsInventoryOpen()
{
	if (InventoryComponent) {
		return InventoryComponent->IsInventoryOpen();
	}
	return false;
}

void AJPlayableCharacter::Interact(const FInputActionValue& Value)
{
	
	// 상호작용 종료
	if (IsInteractingWithNPC) {
		TerminateInteract();
		return;
	}
		
	FVector Start = GetCameraComponent()->GetComponentLocation();
	FVector End = Start + GetCameraComponent()->GetForwardVector() * 500.f;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *HitResult.GetActor()->GetFName().ToString());
		if (IJInteractableInterface* Interface = Cast<IJInteractableInterface>(HitResult.GetActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Interact"));
			Interface->Interact(this);
			if(AJNonplayableCharacter* NPC = Cast<AJNonplayableCharacter>(HitResult.GetActor()))
			{
				lastInteractedNPC = NPC;
				IsInteractingWithNPC = true;
				// 상호작용 종료시 lastInteractedNPC에 nullptr을 넣어야함
			}
		}
	}
	
}

void AJPlayableCharacter::TerminateInteract()
{
	if (IsInteractingWithNPC)
	{
		IsInteractingWithNPC = false;
		lastInteractedNPC->TerminateInteract(this);

		if(InventoryComponent->IsInventoryOpen())
		{
			ToggleInventory();
		}
		lastInteractedNPC = nullptr;
	}
}

void AJPlayableCharacter::TraceForOutline()
{
	FVector Start = GetCameraComponent()->GetComponentLocation();
	FVector End = Start + GetCameraComponent()->GetForwardVector() * 500.f;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		if (Cast<IJInteractableInterface>(HitResult.GetActor()))
		{
			auto HitTarget = HitResult.GetActor();
			TArray<UStaticMeshComponent*> Components;
			HitTarget->GetComponents<UStaticMeshComponent>(Components);

			// 첫 번째로 찾은 컴포넌트에 접근
			if (Components.Num() > 0)
			{
				UStaticMeshComponent* MeshComponent = Components[0];
				if (MeshComponent)
				{
					if(CurrentTarget && MeshComponent != CurrentTarget)
					{
						CurrentTarget->SetRenderCustomDepth(false);
						CurrentTarget = nullptr;
					}
					CurrentTarget = MeshComponent;
					CurrentTarget->SetRenderCustomDepth(true);
					return;
				}
			}
		}
		if (CurrentTarget)
		{
			CurrentTarget->SetRenderCustomDepth(false);
			CurrentTarget = nullptr;
		}
	}
	
}

void AJPlayableCharacter::SendMyInfoToServer()
{
	if (auto JGameInst = Cast<UJGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		ProjectJ::C_MATCH_INFO SendPacket;
		SendPacket.set_player_index(CharacterIndex);

		auto MyInfo = new ProjectJ::C_MATCH_INFO_MyInfo();
		auto WPosition = new ProjectJ::Vector();
		WPosition->set_x(GetActorLocation().X);
		WPosition->set_y(GetActorLocation().Y);
		WPosition->set_z(GetActorLocation().Z);
		MyInfo->set_allocated_position(WPosition);

		auto WRotator = new ProjectJ::Rotator();
		WRotator->set_pitch(GetActorRotation().Pitch);
		WRotator->set_yaw(GetActorRotation().Yaw);
		WRotator->set_roll(GetActorRotation().Roll);
		MyInfo->set_allocated_rotation(WRotator);

		SendPacket.set_allocated_info(MyInfo);

		auto SendBuffer = UJPacketHandler::MakeSendBuffer(SendPacket);
		JGameInst->SendAsync(SendBuffer);
	}
}
