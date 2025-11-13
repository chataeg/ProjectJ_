// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/JPlayableCharacter.h"
#include "JFugitiveCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJ_API AJFugitiveCharacter : public AJPlayableCharacter
{
	GENERATED_BODY()

public:
	AJFugitiveCharacter();

public:
	virtual void BeginPlay() override;

public:

	// First person camera
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UCameraComponent> FirstPersonCameraComponent;


protected:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnimation();
};
