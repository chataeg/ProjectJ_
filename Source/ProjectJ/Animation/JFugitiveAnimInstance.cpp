// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/JFugitiveAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UJFugitiveAnimInstance::UJFugitiveAnimInstance()
{
	MovingThreshould = 3.f;

}

void UJFugitiveAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UJFugitiveAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	NormalizedVelocity = Velocity.GetSafeNormal();
	ForwardVector = Owner->GetActorForwardVector();
	RightVector = Owner->GetActorRightVector();

	ForwardSpeed = FVector::DotProduct(NormalizedVelocity, ForwardVector);
	RightSpeed = FVector::DotProduct(NormalizedVelocity, RightVector);

	MovementDirection = FVector2D(ForwardSpeed, RightSpeed);

	AngleWithXAxisDirection = FMath::RadiansToDegrees(FMath::Atan2(MovementDirection.Y, MovementDirection.X));

	// UE_LOG(LogTemp, Warning, TEXT("AngleWithXAxisDirection : %f"), AngleWithXAxisDirection);

	Velocity = Movement->Velocity;

	GroundSpeed = Velocity.Size2D();

	bIsIdle = GroundSpeed < MovingThreshould;
	bIsFalling = Movement->IsFalling();
}
