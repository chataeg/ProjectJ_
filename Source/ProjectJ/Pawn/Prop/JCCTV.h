// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawn/JPawnBase.h"
#include "InputActionValue.h"
#include "JCCTV.generated.h"

// CCTV 
// 최초 작성자: 신동민
// 수정자:
// 최종 수정일: 

UCLASS()
class PROJECTJ_API AJCCTV : public AJPawnBase
{
	GENERATED_BODY()

public:
	AJCCTV();

protected:
	virtual void BeginPlay() override;

protected:
	// virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> BodyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> CCTVCameraComponent;

	
	// 카메라 초기 회전 설정 함수
	UFUNCTION(BlueprintCallable, Category= Camera)
	void SetCameraRelativeRotation(FRotator NewRotation);

protected:
	

	// void LookCCTV(const FInputActionValue& Value);

protected:
};
