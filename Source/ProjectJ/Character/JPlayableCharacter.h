// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/JCharacterBase.h"
#include "InputActionValue.h"
#include "JNonplayableCharacter.h"

#include "JPlayableCharacter.generated.h"




// Playable Character (Chaser, Fugitive의 부모 클래스)
// 최초 작성자: 김기홍
// 수정자: 
// 최종 수정일: 2023-11-13

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

UCLASS()
class PROJECTJ_API AJPlayableCharacter : public AJCharacterBase
{
	GENERATED_BODY()


public:

	// MappingContext
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	// Move Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

public:
	AJPlayableCharacter();

	// Initailize
	virtual void InitializeCharacter(int _CharacterIndex) override;

protected:
	virtual void BeginPlay() override;

public:
	// Look Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;
	
	// ToggleInventory Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ToggleInventoryAction;

	// Interact Input Action 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InteractAction;

protected:
	// Called for movement input
	virtual void Move(const FInputActionValue& Value);

	// Called for looking input 
	virtual void Look(const FInputActionValue& Value);

	// Called for ToggleInventory input
	void ToggleInventory();
	bool IsInventoryOpen();

	// Called for Interact input
	void Interact(const FInputActionValue& Value);

	// Called for Terminate Interact input
	void TerminateInteract();

	// Show Outline
	void TraceForOutline();

	void SendMyInfoToServer();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

protected:
	// virtual void SetCameraComponent(UCameraComponent* FirstPersonCameraComponent);
	virtual UCameraComponent* GetCameraComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<class UCameraComponent> FirstPersonCameraComponent;

protected:
	bool IsForcedOpen = false;
	bool IsInteractingWithNPC = false;
	AJNonplayableCharacter* lastInteractedNPC;
	UStaticMeshComponent* CurrentTarget = nullptr;

};
