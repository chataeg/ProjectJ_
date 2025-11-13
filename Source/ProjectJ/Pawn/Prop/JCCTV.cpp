// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/Prop/JCCTV.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Components/StaticMeshComponent.h"
#include "Character/JChaserCharacter.h"


AJCCTV::AJCCTV()
{
	PrimaryActorTick.bCanEverTick = true;
	// Mesh
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CCTVMesh"));

	/*Root*/
	RootComponent = BodyMesh;

	/*
	// U 모양 CCTV
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMeshRef(TEXT("/Game/Assets/Meshes/CCTV/SM_CCTV.SM_CCTV"));
	if(BodyMeshRef.Object)
	{
		BodyMesh->SetStaticMesh(BodyMeshRef.Object);
	}
	*/

	// O 모양 CCTV

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMeshRef(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if(BodyMeshRef.Object)
	{
		UE_LOG(LogTemp, Warning, TEXT("Sphere"));
		BodyMesh->SetStaticMesh(BodyMeshRef.Object);
		BodyMesh->SetWorldScale3D(FVector(.05f, .05f, .05f));
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAssetRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Blueprints/MI_CCTV.MI_CCTV'"));
	if (MaterialAssetRef.Object)
	{
		BodyMesh->SetMaterial(0, MaterialAssetRef.Object);
	}
	// Camera
	CCTVCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CCTVCamera"));
	CCTVCameraComponent->SetupAttachment(BodyMesh);
	SetCameraRelativeRotation(FRotator( 0.f, 180.f, 0.f));
	CCTVCameraComponent->bUsePawnControlRotation = true;


	// Input

	//static ConstructorHelpers::FObjectFinder<UInputMappingContext> DEFAULT_CONTEXT
	//(TEXT("/Game/Blueprints/Input/IMC_Default.IMC_Default"));
	//if (DEFAULT_CONTEXT.Succeeded())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Find IMC_Default"));
	//	DefaultMappingContext = DEFAULT_CONTEXT.Object;
	//}

	/*
	
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> CCTVMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Blueprints/Input/IMC_CCTV.IMC_CCTV'"));
	if(CCTVMappingContextRef.Object)
	{
		UE_LOG(LogTemp, Log, TEXT("A"));
		CCTVMappingContext = CCTVMappingContextRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputLookCCTVActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/Actions/IA_LookCCTV.IA_LookCCTV'"));
	if(InputLookCCTVActionRef.Object)
	{
		LookCCTVAction = InputLookCCTVActionRef.Object;
	}

	*/
}

void AJCCTV::BeginPlay()
{
	Super::BeginPlay();
	/*
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CCTVMappingContext, 0);
		}
	}
	*/
}

/*
void AJCCTV::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	
	//// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(LookCCTVAction, ETriggerEvent::Triggered, this, &AJCCTV::LookCCTV);
		
	}
	
}
*/

void AJCCTV::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AJCCTV::SetCameraRelativeRotation(FRotator NewRotation)
{
	if(CCTVCameraComponent)
	{
		CCTVCameraComponent->SetRelativeRotation(NewRotation);
	}
}

/*
void AJCCTV::LookCCTV(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	UE_LOG(LogTemp, Warning, TEXT("Looking CCTV"));

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);

		UE_LOG(LogTemp, Warning, TEXT("Looking CCTV"));
	}
}
*/
