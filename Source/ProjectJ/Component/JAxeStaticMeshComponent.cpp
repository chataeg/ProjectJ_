// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/JAxeStaticMeshComponent.h"

UJAxeStaticMeshComponent::UJAxeStaticMeshComponent()
{
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Character/Chaser/Meshes/Weapon/SM_Fire_Axe.SM_Fire_Axe'"));
	if (BodyMeshRef.Object)
	{
		SetStaticMesh(BodyMeshRef.Object);
	}
}
