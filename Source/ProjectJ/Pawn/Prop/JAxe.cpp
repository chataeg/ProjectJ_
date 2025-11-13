// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/Prop/JAxe.h"

AJAxe::AJAxe()
{
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CCTVMesh"));

	RootComponent = BodyMesh;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Character/Chaser/Meshes/Weapon/SM_Fire_Axe.SM_Fire_Axe'"));
	if(BodyMeshRef.Object)
	{
		BodyMesh->SetStaticMesh(BodyMeshRef.Object);
					
	}

}

void AJAxe::BeginPlay()
{
	Super::BeginPlay();
}
