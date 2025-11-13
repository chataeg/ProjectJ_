// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/JNonplayableCharacter.h"
#include "Interface/JInteractableInterface.h"
#include "JScale.generated.h"

// Non Playable Character를 상속받은 저울
// 최초 작성자: 김기홍
// 수정자: 
// 최종 수정일: 2023-11-13
UCLASS()
class PROJECTJ_API AJScale : public AJNonplayableCharacter
{
	GENERATED_BODY()

public:
	AJScale();

private:
	// 0(false), 1(true, ing), 2(true, end)
	int IsOpen;
	FVector StartingLocation;
	float MoveSpeed = 15;
	float MoveDistance = 300;

	class USoundWave* MovingSound;

	AJPlayableCharacter* LastInteractCharacter;

	UPROPERTY(VisibleAnywhere, Category = "ActorMeshComponents")
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(VisibleAnywhere, Category = "ActorMeshComponents")
	UAudioComponent* AudioComponent;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// Called for ToggleInventory input
	void ToggleInventory();
	bool IsInventoryOpen();

public:
	void OpenDoor();
	bool IsOperating() { return IsOpen > 0; }

	virtual void InitializeCharacter(int _CharacterIndex) override;

	// UJInteractableInterface interface
	virtual void Interact(AJPlayableCharacter* Character) override;
	virtual void TerminateInteract(AJPlayableCharacter* Character) override;
	// End of UJInteractableInterface interface
};
