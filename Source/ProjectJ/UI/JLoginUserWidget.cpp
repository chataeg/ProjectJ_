// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/JLoginUserWidget.h"
#include "Network/JHttpObject.h"
#include "Network/JSendBuffer.h"
#include "Player/JPlayerController.h"
#include "Network/JSocket.h"
#include "Network/JSession.h"
#include "Network/JRecvThread.h"
#include "Network/JSendThread.h"
#include "Network/Protocol/JPackets.h"
#include "../Network/JGameInstance.h"

UJLoginUserWidget::UJLoginUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("Constructor"));
	JGameInst = Cast<UJGameInstance>(GetGameInstance());
}

void UJLoginUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Warning, TEXT("Native Constructor"));

	UJPacketHandler::Packet_S_VERIFY_TOKEN_Delegate.BindUObject(this, &UJLoginUserWidget::LoginVerified);
	DelegateHandle = JGameInst->GetHttpObject()->HttpResponseOkDelegate.AddUObject(this, &UJLoginUserWidget::LoginResponseOk);
	JGameInst->GetHttpObject()->HttpResponseDeniedDelegate.BindUObject(this, &UJLoginUserWidget::LoginResponseUnauthorized);

	JGameInst->GetHttpObject()->HttpResponseBadRequestDelegate.BindUObject(this, &UJLoginUserWidget::SignUpResponseBadRequest);
	JGameInst->GetHttpObject()->HttpResponseConflictDelegate.BindUObject(this, &UJLoginUserWidget::SignUpResponseConflict);
	JGameInst->GetHttpObject()->HttpResponseCreatedDelegate.BindUObject(this, &UJLoginUserWidget::SignUpResponseCreated);
	JGameInst->GetHttpObject()->HttpResponseInvalidDelegate.BindUObject(this, &UJLoginUserWidget::HttpResponseInvalid);

	//FInputModeUIOnly InputData;
	//InputData.SetWidgetToFocus(JGameInst->LoginWidgetInstance->TakeWidget());
	//UGameplayStatics::GetPlayerController(GetWorld(), JGameInst->GetSession()->GetId())->SetInputMode(InputData);
}

void UJLoginUserWidget::NativeDestruct()
{
	Super::NativeDestruct();

	//UnBind

	UJPacketHandler::Packet_S_VERIFY_TOKEN_Delegate.Unbind();
	JGameInst->GetHttpObject()->HttpResponseOkDelegate.Remove(DelegateHandle);
	JGameInst->GetHttpObject()->HttpResponseCreatedDelegate.Unbind();
	JGameInst->GetHttpObject()->HttpResponseBadRequestDelegate.Unbind();
	JGameInst->GetHttpObject()->HttpResponseDeniedDelegate.Unbind();
	JGameInst->GetHttpObject()->HttpResponseInvalidDelegate.Unbind();

	UE_LOG(LogTemp, Warning, TEXT("UnBind"));
}


void UJLoginUserWidget::LoginRequested(const FString& Name, const FString& Password)
{
	UE_LOG(LogTemp, Warning, TEXT("LoginRequest entered"));
	if (JGameInst)
	{
		JGameInst->GetHttpObject()->PostLoginRequest(Name, Password);
		UE_LOG(LogTemp, Warning, TEXT("LoginRequested"));
	}
}

void UJLoginUserWidget::LoginResponseOk(const FHttpRequestPtr& Request, const FHttpResponsePtr& Response)
{
}

void UJLoginUserWidget::LoginResponseConflict(const FHttpRequestPtr& Request, const FHttpResponsePtr& Response)
{
	SetDetail(ParseDetail(Response));
	PopMessage(FText::FromString(GetDetail()));
}

void UJLoginUserWidget::LoginResponseUnauthorized(const FHttpRequestPtr& Request, const FHttpResponsePtr& Response)
{
	SetDetail(ParseDetail(Response));
	PopMessage(FText::FromString(GetDetail()));
}

void UJLoginUserWidget::HttpResponseInvalid()
{
	UE_LOG(LogTemp, Warning, TEXT("HttpResponse Invalid"));
}

FString UJLoginUserWidget::ParseDetail(const FHttpResponsePtr& Response)
{
	// JSON 파싱
	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, ResponseObj))
	{
		// const FString
		Detail = ResponseObj->GetStringField(TEXT("detail"));
	}

	return Detail;
}

void UJLoginUserWidget::SignUpRequested(const FString& Name, const FString& Password, const FString& Nickname)
{
	UE_LOG(LogTemp, Warning, TEXT("SignUpRequest entered"));
	if (JGameInst)
	{
		JGameInst->GetHttpObject()->PostCreateRequest(Name, Nickname, Password);
		UE_LOG(LogTemp, Warning, TEXT("SignUpRequested"));
	}
}

void UJLoginUserWidget::SignUpResponseCreated(const FHttpRequestPtr& Request, const FHttpResponsePtr& Response)
{
	PopMessage(FText::FromString(TEXT("회원가입 성공!")));

	UE_LOG(LogTemp, Warning, TEXT("SignUp : RemoveFromViewport"));
}

void UJLoginUserWidget::SignUpResponseBadRequest(const FHttpRequestPtr& Request, const FHttpResponsePtr& Response)
{
	UE_LOG(LogTemp, Warning, TEXT("SignUp : BadRequest"));
	SetDetail(ParseDetail(Response));
	PopMessage(FText::FromString(GetDetail()));
}

void UJLoginUserWidget::SignUpResponseConflict(const FHttpRequestPtr& Request, const FHttpResponsePtr& Response)
{
	UE_LOG(LogTemp, Warning, TEXT("SignUp : ResponseConflict"));
	SetDetail(ParseDetail(Response));
	PopMessage(FText::FromString(GetDetail()));
}

bool UJLoginUserWidget::LoginVerified(UWorld* World, ProjectJ::S_VERIFY_TOKEN& Packet, float DeltaSeconds)
{
	if (Packet.result())
	{
		//로비 진입 
		UE_LOG(LogTemp, Warning, TEXT("Token Verified!"));
		// 플레이어 컨트롤러 참조 가져오기
		APlayerController* PC = JGameInst->GetFirstLocalPlayerController();
		AJPlayerController* JPC = Cast<AJPlayerController>(PC);
		//기본 폰에 포커스 

		/*
		FInputModeUIOnly InputModeData;
		InputModeData.SetWidgetToFocus(JGameInst->LobbyWidgetInstance->TakeWidget());
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		JPC->SetInputMode(InputModeData);
		JPC->bShowMouseCursor = true; // 마우스 커서를 화면에서 숨기려면 false로 설정
		*/

		auto JGameMode = Cast<AJGameMode>(GetWorld()->GetAuthGameMode());

		JGameMode->SetPlayerState(EGameState::Lobby);

		UE_LOG(LogTemp, Warning, TEXT("RemoveLoginWidgetFromViewport"));
		if (AJPlayerController* PlayerController = Cast<AJPlayerController>(GetOwningPlayer()))
		{
			PlayerController->LoginWidgetInstance->RemoveFromViewport();

			PlayerController->LobbyWidgetInstance = CreateWidget(PlayerController, PlayerController->LobbyWidgetClass);
			if (PlayerController->LobbyWidgetInstance)
			{
				PlayerController->LobbyWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		JGameInst->StopThreads();
	}

	return true;
}


FString UJLoginUserWidget::GetDetail()
{
	return Detail;
}

void UJLoginUserWidget::SetDetail(FString input)
{
	Detail = input;
}

void UJLoginUserWidget::PopMessage(FText Message)
{
	if (AJPlayerController* PlayerController = Cast<AJPlayerController>(GetOwningPlayer()))
	{
		PlayerController->PopUpWidgetInstance = CreateWidget(PlayerController, PlayerController->PopUpWidgetClass);
		if (PlayerController->PopUpWidgetInstance)
		{
			if (auto PopUpWidget = Cast<UTextBlock>(PlayerController->PopUpWidgetInstance->GetWidgetFromName("Msg")))
			{
				PopUpWidget->SetText(Message);
				PlayerController->PopUpWidgetInstance->AddToViewport();
			}
		}
	}
}
