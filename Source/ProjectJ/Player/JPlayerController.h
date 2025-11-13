// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../JGameMode.h"
#include "Blueprint/UserWidget.h"
#include "../JGameMode.h"
#include "ProjectJ/FJItemManager.h"
#include "ProjectJ/FJCharacterManager.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "JPlayerController.generated.h"

// 플레이어 컨트롤러 
// 최초 작성자: 차태관
// 수정자: 김기홍, 박별
// 최종 수정일: 2023-11-20
UCLASS()
class PROJECTJ_API AJPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AJPlayerController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	virtual void SetupInputComponent() override;
	void OnEnterPressed();

	UFUNCTION(BlueprintCallable)
	void OnSendChat(const FString& Msg);

	UFUNCTION(BlueprintCallable)
	void OnRecvChat(UScrollBox* ChatLog, FText Message, bool IsSystem = false);

	AGameModeBase* GetGameModeBase() const { return Cast<AJGameMode>(GetWorld()->GetAuthGameMode()); }
	UJGameInstance* GetGameInstance() const { return Cast<UJGameInstance>(GetGameModeBase()->GetGameInstance()); }

public:
	UPROPERTY()
	TSubclassOf<class UUserWidget> LoginWidgetClass;

	UPROPERTY()
	UUserWidget* LoginWidgetInstance;

	UPROPERTY()
	TSubclassOf<class UUserWidget> SettingsWidgetClass;

	UPROPERTY()
	UUserWidget* SettingsWidgetInstance;

	UPROPERTY()
	TSubclassOf<class UUserWidget> PopUpWidgetClass;

	UPROPERTY()
	UUserWidget* PopUpWidgetInstance;

	UPROPERTY()
	TSubclassOf<class UUserWidget> LobbyWidgetClass;

	UPROPERTY()
	UUserWidget* LobbyWidgetInstance;

	UPROPERTY()
	TSubclassOf<class UUserWidget> ChatWidgetClass;

	UPROPERTY()
	UUserWidget* ChatWidgetInstance;

	UPROPERTY()
	TSubclassOf<class UUserWidget> ChatListWidgetClass;

	UPROPERTY()
	UUserWidget* ChatListWidgetInstance;

	UPROPERTY()
	TSubclassOf<class UUserWidget> RoomWidgetClass;

	UPROPERTY()
	UUserWidget* RoomWidgetInstance;

protected:
	UPROPERTY()
	AJGameMode* JGameMode;

	UPROPERTY()
	UJGameInstance* JGameInst;

	std::unique_ptr<FJItemManager> ItemManager;
	std::unique_ptr<FJCharacterManager> CharacterManager;
};
