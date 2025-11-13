// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Character/JPlayableCharacter.h"
#include "Component/JAxeStaticMeshComponent.h"
#include "Interface/JAnimationAttackInterface.h"
#include "Pawn/Prop/JCCTV.h"
#include "Pawn/Prop/JCCTVPreview.h"
#include "JChaserCharacter.generated.h"

// Chaser Character
// 최초 작성자: 신동민
// 수정자:
// 최종 수정일:


UENUM()
enum class EChaserStateType : uint8
{
	Nomal ,
	SetCamera,
	ViewCamera,
};

UCLASS()
class PROJECTJ_API AJChaserCharacter : public AJPlayableCharacter, public IJAnimationAttackInterface
{
	GENERATED_BODY()

	
public:
	AJChaserCharacter();

public:
	virtual void BeginPlay() override;

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UJAxeStaticMeshComponent> AxeComponent;	

	// First person camera
	
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<class UCameraComponent> FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SwitchViewAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SpawnCCTVorAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SetCamModeAction;

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<class UAnimSequence> AttackAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ObjectSpawner")
	TObjectPtr<class AJCCTVPreview> ObjectPreview;
	
protected:
			
	void SwitchView(const FInputActionValue& Value);

	void SettingMode(const FInputActionValue& Value);

	void SpawnCCTVorAttack(const FInputActionValue& Value);

	// void PreviewSetting(const FInputActionValue& Value);

	float MaxPitch = 45.f;
	float MinPitch = 315.f; // 360 - 45;
	
	virtual void Look(const FInputActionValue& Value) override;

	virtual void Move(const FInputActionValue& Value) override;

	ACharacter* OriginalCharacter;

	float SpawnDistance;

	bool bHasAttackAnimationStarted;
	bool bIsAnimationPlaying;

	TArray<AJCCTV*> SpawnedActors;
	int32 SpawnedCnt = 0;
	const int32 MaxCount = 3;
	int32 SelectedCCTVIndex = 0;

	EChaserStateType CurrentChaserState;

	//Attack
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackMontage;

	void ProcessAttack();

	void AttackBegin();
	void AttackEnd(class UAnimMontage* TargetMontage, bool IsPreperlyEnded); // 몽타주에 설정된 델리게이트를 통해 바로 호출될 수 있도록 파라미터를 맞춰보기

	bool bIsAttaking = false;

	// Attack Hit
protected:
	virtual void AttackHitCheck() override;
	virtual UCameraComponent* GetCameraComponent() override;
	// virtual void SetCameraComponent(UCameraComponent* FirstPersonCameraComponent) override;
};
