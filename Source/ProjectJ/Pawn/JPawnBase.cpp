// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/JPawnBase.h"

// Sets default values
AJPawnBase::AJPawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJPawnBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJPawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AJPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

