// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/JCharacterBase.h"
#include "Component/JInventoryComponent.h"
#include "Interface/JInteractableInterface.h"
#include "JNonplayableCharacter.generated.h"

// NPC의 parent class
// 최초 작성자 : 김기홍
// 수정자 : 
// 최종 수정일 : 2023-11-02
UCLASS()
class PROJECTJ_API AJNonplayableCharacter : public AJCharacterBase, public IJInteractableInterface
{
	GENERATED_BODY()

public:
	// UJInteractableInterface interface
	virtual void Interact(AJPlayableCharacter* Character) override{};
	virtual void TerminateInteract(AJPlayableCharacter* Character) override{};
	// End of UJInteractableInterface interface
};
