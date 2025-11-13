// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/JPlayerController.h"
#include "JGameMode.h"
#include "Network/JPacketHandler.h"
#include "Network/JGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableText.h"
#include "Components/Button.h"

AJPlayerController::AJPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> LoginWidget(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/BP_Login.BP_Login_C'"));
	if (LoginWidget.Succeeded())
	{
		LoginWidgetClass = LoginWidget.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> SettingsWidget(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/BP_Settings.BP_Settings_C'"));
	if (SettingsWidget.Succeeded())
	{
		SettingsWidgetClass = SettingsWidget.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> PopUpWidget(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/BP_PopUp.BP_PopUp_C'"));
	if (PopUpWidget.Succeeded())
	{
		PopUpWidgetClass = PopUpWidget.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> LobbyWidget(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/BP_Lobby.BP_Lobby_C'"));
	if (LobbyWidget.Succeeded())
	{
		LobbyWidgetClass = LobbyWidget.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> ChatWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/BP_Chat.BP_Chat_C'"));
	if (ChatWidget.Succeeded())
	{
		ChatListWidgetClass = ChatWidget.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> RoomWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/BP_Room.BP_Room_C'"));
	if (RoomWidget.Succeeded())
	{
		RoomWidgetClass = RoomWidget.Class;
	}
}

void AJPlayerController::BeginPlay()
{
	Super::BeginPlay();

	JGameInst = GetGameInstance();
	JGameMode = Cast<AJGameMode>(GetWorld()->GetAuthGameMode());
	JGameMode->SetPlayerState(EGameState::Login);


	LoginWidgetInstance = CreateWidget<UUserWidget>(this, LoginWidgetClass);
	if (LoginWidgetInstance)
	{
		LoginWidgetInstance->AddToViewport();

		FInputModeUIOnly InputModeData;
		InputModeData.SetWidgetToFocus(LoginWidgetInstance->TakeWidget());
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(InputModeData);
		bShowMouseCursor = true; // 마우스 커서를 화면에서 숨기려면 false로 설정
	}
}

void AJPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AJPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UJPacketHandler::Packet_S_LOBBY_CHAT_Delegate.Unbind();
	UJPacketHandler::Packet_S_ROOM_CHAT_Delegate.Unbind();
}

void AJPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//InputComponent->BindAction("Enter", IE_Pressed, this, &AJPlayerController::OnEnterPressed);
}

void AJPlayerController::OnEnterPressed()
{
	UE_LOG(LogTemp, Log, TEXT("Enter Pressed"));
}


void AJPlayerController::OnSendChat(const FString& Msg)
{
	UE_LOG(LogTemp, Warning, TEXT("OnSendChat called"));

	JGameInst = GetGameInstance();

	if (JGameMode->GetPlayerState() == EGameState::Lobby)
	{
		ProjectJ::C_LOBBY_CHAT Chat;

		FString ChatString(Msg);

		Chat.set_chat(TCHAR_TO_UTF8(*ChatString));

		if (JGameInst)
		{
			Chat.set_nickname(TCHAR_TO_UTF8(*JGameInst->GetSession()->GetNickname()));

			Chat.set_account_id(JGameInst->GetSession()->GetID());
		}
		auto SendBuffer = UJPacketHandler::MakeSendBuffer(Chat);

		if (JGameInst)
		{
			JGameInst->SendAsync(SendBuffer);
		}
	}
	else if (JGameMode->GetPlayerState() == EGameState::Room)
	{
		ProjectJ::C_ROOM_CHAT Chat;

		FString ChatString(Msg);

		Chat.set_chat(TCHAR_TO_UTF8(*ChatString));

		if (JGameInst)
		{
			Chat.set_nickname(TCHAR_TO_UTF8(*JGameInst->GetSession()->GetNickname()));

			Chat.set_account_id(JGameInst->GetSession()->GetID());
		}
		auto SendBuffer = UJPacketHandler::MakeSendBuffer(Chat);

		if (JGameInst)
		{
			JGameInst->SendAsync(SendBuffer);
		}
	}
}

void AJPlayerController::OnRecvChat(UScrollBox* ChatLog, FText Message, bool IsSystem)
{
	UE_LOG(LogTemp, Warning, TEXT("Chatlog  valid"));

	UTextBlock* newTextBlock;
	newTextBlock = NewObject<UTextBlock>();

	UObject* FontObj = LoadObject<UObject>(
		this, TEXT("/Script/Engine.Font'/Game/Assets/Fonts/NEXON_Lv1_Gothic_Low_OTF_Font.NEXON_Lv1_Gothic_Low_OTF_Font'"), NULL, LOAD_None, NULL);
	FSlateFontInfo SlateFontInfo(FontObj, 24);

	newTextBlock->SetFont(SlateFontInfo);

	if (IsSystem)
	{
		newTextBlock->SetColorAndOpacity(FColor::Cyan);
	}


	newTextBlock->SetText(Message);
	ChatLog->AddChild(newTextBlock);
}
