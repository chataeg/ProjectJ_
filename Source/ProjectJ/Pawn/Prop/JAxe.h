// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawn/JPawnBase.h"
#include "JAxe.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJ_API AJAxe : public AJPawnBase
{
	GENERATED_BODY()

public:
	AJAxe();

protected:
	virtual void BeginPlay() override;

protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> BodyMesh;

};
