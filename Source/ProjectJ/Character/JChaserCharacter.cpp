// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/JChaserCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Pawn/Prop/JCCTV.h"
#include "Component/JAxeStaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Physics/JCollision.h"

AJChaserCharacter::AJChaserCharacter()
{
	// 캐릭터 상태
	CurrentChaserState = EChaserStateType::Nomal;

	// CCTV 스폰 거리
	SpawnDistance = 400.f;

	bHasAttackAnimationStarted = false;
	bIsAnimationPlaying = false;

	OriginalCharacter = this;



	// 스켈레탈 메시
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Character/Chaser/Meshes/RedNeck.RedNeck'"));
	if(MeshRef.Object)
	{

		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -99.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	// 카메라 컴포넌트

	//FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	//FirstPersonCameraComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "Head");
	FirstPersonCameraComponent->SetupAttachment(GetMesh(), FName("Head"));
	// 0.f, -45.f, 70.f
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.f, -50.f, 80.f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// 도끼 메시
	AxeComponent = CreateDefaultSubobject<UJAxeStaticMeshComponent>(TEXT("AxeMesh"));
	// AxeComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "RightHand");
	AxeComponent->SetupAttachment(GetMesh(),FName("RightHand"));
	AxeComponent->SetRelativeLocationAndRotation(FVector(10.f, -45.f, 15.f), FRotator(0, 90.f, -90.f));
	AxeComponent->SetRelativeScale3D(FVector(5.f, 5.f, 5.f));

	// 애니메이션
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Animation/Chaser/ABP_Chaser.ABP_Chaser_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Chaser/AM_Attack.AM_Attack'"));
	if(AttackMontageRef.Object)
	{
		AttackMontage = AttackMontageRef.Object;
	}
		
	// input

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DEFAULT_CONTEXT(TEXT("/Game/Blueprints/Input/IMC_Default.IMC_Default"));
	if (DEFAULT_CONTEXT.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Find IMC_Default"));
		DefaultMappingContext = DEFAULT_CONTEXT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Switch(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/Actions/IA_SwitchView.IA_SwitchView'"));
	if (IA_Switch.Object)
	{
		SwitchViewAction = IA_Switch.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_SetCamMode(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/Actions/IA_SetCamMode.IA_SetCamMode'"));
	if (IA_SetCamMode.Object)
	{
		SetCamModeAction = IA_SetCamMode.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Spawn_Attack(TEXT("/Script/EnhancedInput.InputAction'/Game/Blueprints/Input/Actions/IA_Spawn_Attack.IA_Spawn_Attack'"));
	if(IA_Spawn_Attack.Object)
	{
		SpawnCCTVorAttackAction = IA_Spawn_Attack.Object;
	}

}

void AJChaserCharacter::BeginPlay()
{
	Super::BeginPlay();

	ObjectPreview = GetWorld()->SpawnActor<AJCCTVPreview>(AJCCTVPreview::StaticClass(), FVector(FMath::RandRange( 5000.f, 6000.f), 10000.f, 5000.f), FRotator::ZeroRotator);
	UE_LOG(LogTemp, Warning, TEXT("CCTV : %p"), ObjectPreview);
	if (ObjectPreview)
	{
		ObjectPreview->SetActorEnableCollision(false);
		ObjectPreview->SetActorHiddenInGame(false);
		
	}
}

void AJChaserCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// SwitchView
		EnhancedInputComponent->BindAction(SwitchViewAction, ETriggerEvent::Started, this, &AJChaserCharacter::SwitchView);

		// Setting Mode
		EnhancedInputComponent->BindAction(SetCamModeAction, ETriggerEvent::Started, this, &AJChaserCharacter::SettingMode);

		// Spawn
		EnhancedInputComponent->BindAction(SpawnCCTVorAttackAction, ETriggerEvent::Started, this, &AJChaserCharacter::SpawnCCTVorAttack);

		
	}
}

void AJChaserCharacter::SwitchView(const FInputActionValue& Value)
{
	if(CurrentChaserState == EChaserStateType::SetCamera)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current State is SetCam, So this input is none"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Switch V"));
	APlayerController* PlayerController =Cast<APlayerController>(Controller);
	
	UE_LOG(LogTemp, Warning, TEXT("%p"), PlayerController);
	// float BlendTime = 0.f;
	// TEnumAsByte<EViewTargetBlendFunction> BlendFunction = VTBlend_Linear;

	if (SelectedCCTVIndex >= 0 && SelectedCCTVIndex < SpawnedActors.Num())
	{	
		AJCCTV* SelectedCCTV = SpawnedActors[SelectedCCTVIndex];
		if(PlayerController != nullptr)
		{
			PlayerController->SetViewTarget(SelectedCCTV);
		}
		

		UE_LOG(LogTemp, Warning, TEXT("Switch Success: %d"), SelectedCCTVIndex);

		CurrentChaserState = EChaserStateType::ViewCamera;
		UE_LOG(LogTemp, Warning, TEXT("ViewCam State"));
		
		SelectedCCTVIndex++;		
	}
	else
	{
		PlayerController->SetViewTarget(OriginalCharacter);
		
		//OriginalCharacter->FindComponentByClass<UCameraComponent>()->SetRelativeLocation(FVector(10.f, 0.f, 60.f));
		//UCameraComponent* CameraComponent = OriginalCharacter->FindComponentByClass<UCameraComponent>();
		//CameraComponent->SetRelativeLocation(FVector(10.f, 0.f, 60.f));
		
		SelectedCCTVIndex = 0;
		CurrentChaserState = EChaserStateType::Nomal;
		UE_LOG(LogTemp, Warning, TEXT("Nomal State"));
	}
	
}

void AJChaserCharacter::SettingMode(const FInputActionValue& Value)
{
	if(CurrentChaserState == EChaserStateType::ViewCamera)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current State is ViewCam, So this input is none"));
		return;
	}
	if(CurrentChaserState == EChaserStateType::Nomal)
	{
		CurrentChaserState = EChaserStateType::SetCamera;
		UE_LOG(LogTemp, Warning, TEXT("SetCamState"));
	}
	else if(CurrentChaserState == EChaserStateType::SetCamera)
	{
		CurrentChaserState = EChaserStateType::Nomal;
		UE_LOG(LogTemp, Warning, TEXT("NomalState"));
	}
}

void AJChaserCharacter::SpawnCCTVorAttack(const FInputActionValue& Value)
{
	if(CurrentChaserState == EChaserStateType::SetCamera)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn C"));

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		FVector PlayerLocation = PlayerController->GetPawn()->GetActorLocation();
		FRotator PlayerRotation = PlayerController->GetControlRotation();

		FVector TraceStart = PlayerLocation;
		FVector TraceEnd = PlayerLocation + PlayerRotation.Vector() * SpawnDistance;

		/*
		if (SpawnedCnt < MaxCount)
		{
			AJCCTV* NewCCTV = GetWorld()->SpawnActor<AJCCTV>(AJCCTV::StaticClass(), TraceEnd, PlayerRotation);
			if (NewCCTV)
			{
				SpawnedActors.Add(NewCCTV); // 액터를 TArray에 추가
				UE_LOG(LogTemp, Warning, TEXT("Spawn Success"));
			}
			SpawnedCnt++;

		}
		*/
		
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);
		if(GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams))
		{
			FVector SpawnLocation = HitResult.Location + HitResult.Normal * 3;
			if (SpawnedCnt < MaxCount)
			{
				AJCCTV* NewCCTV = GetWorld()->SpawnActor<AJCCTV>(AJCCTV::StaticClass(), SpawnLocation, PlayerRotation);
				NewCCTV->SetActorEnableCollision(false);
				if (NewCCTV)
				{
					SpawnedActors.Add(NewCCTV); // 액터를 TArray에 추가
					UE_LOG(LogTemp, Warning, TEXT("Spawn Success"));
					SpawnedCnt++;
				}
				

			}
		}
		else
		{
			////////////////////////////////////////////////////////
			// 공중에 뜨게 하려면 주석 풀기
			/*
			if (SpawnedCnt < MaxCount)
			{
				AJCCTV* NewCCTV = GetWorld()->SpawnActor<AJCCTV>(AJCCTV::StaticClass(), TraceEnd, PlayerRotation);
				if (NewCCTV)
				{
					SpawnedActors.Add(NewCCTV); // 액터를 TArray에 추가
					UE_LOG(LogTemp, Warning, TEXT("Spawn Success"));
					SpawnedCnt++;
				}


			}
			*/
		}
		
	}
	else if(CurrentChaserState == EChaserStateType::Nomal)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack"));
		ProcessAttack();
	}
}
/*
void AJChaserCharacter::PreviewSetting(const FInputActionValue& Value)
{
	if (CurrentChaserState == EChaserStateType::SetCamera)
	{
		
	}
}
*/
void AJChaserCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if(CurrentChaserState == EChaserStateType::Nomal || CurrentChaserState == EChaserStateType::SetCamera)
	{
		if (Controller != nullptr)
		{
			// add yaw and pitch input to controller
						
			AddControllerYawInput(LookAxisVector.X);
			AddControllerPitchInput(LookAxisVector.Y);					
			// UE_LOG(LogTemp, Warning, TEXT("%f  ::  %f"), GetControlRotation().Yaw, GetControlRotation().Pitch);
						
			// AddControllerPitchInput(LookAxisVector.Y * GetWorld()->GetDeltaSeconds());
			// UE_LOG(LogTemp, Warning, TEXT("%f     %f"), MinPitch, MaxPitch);
			// UE_LOG(LogTemp, Warning, TEXT("Nomal Look Mode"));
		}
		// UE_LOG(LogTemp, Warning, TEXT("Controller : %p"), Controller);
	}
	else if(CurrentChaserState == EChaserStateType::ViewCamera)
	{
		// UE_LOG(LogTemp, Warning, TEXT("CCTV Look Mode"));
		// UE_LOG(LogTemp, Warning, TEXT("View: %d"), SelectedCCTVIndex-1);
		if(Controller != nullptr)
		{
			AJCCTV* SelectedCCTV = SpawnedActors[SelectedCCTVIndex-1];

			UCameraComponent* CameraComponent = SelectedCCTV->FindComponentByClass<UCameraComponent>();
			if(CameraComponent)
			{
				FRotator CurrentRotation = CameraComponent->GetComponentRotation();

				FRotator NewRotation = CurrentRotation;
				NewRotation.Yaw += LookAxisVector.X;
				NewRotation.Pitch -= LookAxisVector.Y;

				CameraComponent->SetWorldRotation(NewRotation);
			}
		}

	}

	if (CurrentChaserState == EChaserStateType::SetCamera)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		FVector PlayerLocation = PlayerController->GetPawn()->GetActorLocation();
		FRotator PlayerRotation = PlayerController->GetControlRotation();

		FVector TraceStart = PlayerLocation;
		FVector TraceEnd = PlayerLocation + PlayerRotation.Vector() * SpawnDistance;

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		

		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams))
		{
			UE_LOG(LogTemp, Warning, TEXT("QWERTY: %p"), ObjectPreview);
			FVector SpawnLocation = HitResult.Location + HitResult.Normal * 3;

			if (ObjectPreview)
			{
				UE_LOG(LogTemp, Warning, TEXT("Preview"));

				ObjectPreview->SetActorLocation(SpawnLocation);
				ObjectPreview->SetActorHiddenInGame(false);
			}

		}
		else
		{
			if (ObjectPreview)
			{
				UE_LOG(LogTemp, Warning, TEXT("Hide on bush"));
				ObjectPreview->SetActorHiddenInGame(true);
			}
			
		}
	}
}

void AJChaserCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	if(CurrentChaserState == EChaserStateType::Nomal || CurrentChaserState == EChaserStateType::SetCamera)
	{
		if (Controller != nullptr)
		{
			if(MovementVector.Y == -1 )
			{
				GetCharacterMovement()->MaxWalkSpeed = 200;
			}
			else if (MovementVector.Y == 0 && MovementVector.X != 0)
			{
				GetCharacterMovement()->MaxWalkSpeed = 200;
			}
			else if(MovementVector.Y == 1 && MovementVector.X != 0)
			{
				GetCharacterMovement()->MaxWalkSpeed = 400;
			}			
			else
			{
				GetCharacterMovement()->MaxWalkSpeed = 400;
			}
			// add movement 
			AddMovementInput(GetActorForwardVector(), MovementVector.Y);
			AddMovementInput(GetActorRightVector(), MovementVector.X);
			// UE_LOG(LogTemp, Warning, TEXT("MovementVector.Y: %f  MovementVector.X: %f"), MovementVector.Y, MovementVector.X);
			// UE_LOG(LogTemp, Warning, TEXT("Move"));
		}
	}
	else
	{
		// UE_LOG(LogTemp, Warning, TEXT("Current State is not Nomal, So this input is none"));

		return;
	}
	
}

void AJChaserCharacter::ProcessAttack()
{
	if(bIsAttaking == false)
	{
		AttackBegin();
	}
	
}

void AJChaserCharacter::AttackBegin()
{

	bIsAttaking = true;

	GetCharacterMovement()->SetMovementMode(MOVE_None);

	const float AttackSpeedRate = 1.f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AttackMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AJChaserCharacter::AttackEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);
}

void AJChaserCharacter::AttackEnd(UAnimMontage* TargetMontage, bool IsPreperlyEnded)
{
	ensure(bIsAttaking != false);
	bIsAttaking = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

}

void AJChaserCharacter::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), true, this);

	const float AttackRange = 80.f;
	const float AttackRadius = 50.f;
	const float AttackDamage = 30.f;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	// 맞췄을때 ////////////////////////////////////////////////
	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_JACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if(HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);

	}

#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.f);
#endif


}

UCameraComponent* AJChaserCharacter::GetCameraComponent()
{
	return Super::GetCameraComponent();
}
/*
void AJChaserCharacter::SetCameraComponent(UCameraComponent* CameraComponent)
{
	CameraComponent = FirstPersonCameraComponent;
}
*/