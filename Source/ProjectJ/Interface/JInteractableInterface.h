// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "JInteractableInterface.generated.h"


// 상호작용하는 오브젝트들이 상속받아야하는 인터페이스
// 최초 작성자 : 김기홍
// 수정자 : 
// 최종 수정일 : 2023-10-24

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UJInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTJ_API IJInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// PlayableCharacter가 객체와 상호작용할때 호출된다
	// Character: 객체와 상호작용하는 PlayableCharacter
	virtual void Interact(class AJPlayableCharacter* Character) = 0;
	virtual void TerminateInteract(class AJPlayableCharacter* Character) = 0;
};