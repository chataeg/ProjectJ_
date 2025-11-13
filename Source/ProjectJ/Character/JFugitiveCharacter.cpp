// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/JFugitiveCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Physics/JCollision.h"

AJFugitiveCharacter::AJFugitiveCharacter()
{

	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_JCAPSULE);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Bandit_Sport/Mesh/SK_Bandit_Mask.SK_Bandit_Mask'"));
	if (MeshRef.Object)
	{

		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -96.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	// Animation
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Bandit_Sport/Demo/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Bandit_Sport/Demo/Dead.Dead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	// 카메라 컴포넌트
	//FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	//FirstPersonCameraComponent->SetupAttachment(RootComponent);
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// FirstPersonCameraComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "head");
	FirstPersonCameraComponent->SetupAttachment(GetMesh(), FName("head"));
	// 0.f, -45.f, 70.f
	FirstPersonCameraComponent->SetRelativeLocation(FVector(5.f, 15.f, 0.f));
}

void AJFugitiveCharacter::BeginPlay()
{
	Super::BeginPlay();
}

float AJFugitiveCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	SetDead();
	UE_LOG(LogTemp, Warning, TEXT("Take Damage: %f"), DamageAmount);
	return DamageAmount;
}

void AJFugitiveCharacter::SetDead()
{
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);
}

void AJFugitiveCharacter::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.f);
	AnimInstance->Montage_Play(DeadMontage, 1.f);
}
