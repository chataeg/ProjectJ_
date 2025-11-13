// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/JInGameUserWidget.h"

void UJInGameUserWidget::NativeContsruct()
{
	//UJPacketHandler::Packet_S_MATCH_INFO_Delegate.BindUObject(this, &UJInGameUserWidget::OnRecvInfobyTick);

	JGameInst = Cast<UJGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	TimeBlock = Cast<UTextBlock>(GetWidgetFromName("Fugitive1Hpbar"));
	check(TimeBlock);
	Fug1HpBar = Cast<UProgressBar>(GetWidgetFromName("Fugitive1Hpbar"));
	check(Fug1HpBar);
	Fug2HpBar = Cast<UProgressBar>(GetWidgetFromName("Fugitive2Hpbar"));
	check(Fug2HpBar);
	Fug3HpBar = Cast<UProgressBar>(GetWidgetFromName("Fugitive3Hpbar"));
	check(Fug3HpBar);
	LocalCurrentState = Cast<AJInGamePlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
}

void UJInGameUserWidget::NativeDestruct()
{
	GetWorld()->GetTimerManager().ClearTimer(Timer);
}

void UJInGameUserWidget::Init(ProjectJ::S_MATCH_ALL_READY_TO_RECEIVE& Packet)
{
	//시간 설정
	EndTick = 0;
	CurrentTick = 0;
	Time_m = 0;
	Time_s = 0;

	//타이머 설정
	//GetWorld()->GetTimerManager().SetTimer(Timer, this, &UJInGameUserWidget::OnUpdateUIInfo, 1.f, true, 1.f);
	//UI 닉네임 설정
	if (JGameInst)
	{
		if (Packet.info().has_fugitive_first())
		{
			Cast<UTextBlock>(GetWidgetFromName("Fugitive1Name"))->
				SetText(FText::FromString(Packet.info().fugitive_first().player().nickname().c_str()));

			Fug1HpBar->SetPercent(100.f);
		}

		if (Packet.info().has_fugitive_second())
		{
			Cast<UTextBlock>(GetWidgetFromName("Fugitive2Name"))->
				SetText(FText::FromString(Packet.info().fugitive_second().player().nickname().c_str()));

			Fug2HpBar->SetPercent(100.f);
		}

		if (Packet.info().has_fugitive_third())
		{
			Cast<UTextBlock>(GetWidgetFromName("Fugitive3Name"))->
				SetText(FText::FromString(Packet.info().fugitive_third().player().nickname().c_str()));

			Fug3HpBar->SetPercent(100.f);
		}
	}
}

float UJInGameUserWidget::CalHp(const ProjectJ::MatchPlayerState State)
{
	switch (State)
	{
	case ProjectJ::MatchPlayerState::ALIVE:
		return 100.f;
		break;

	case ProjectJ::MatchPlayerState::ALIVE_DAMAGED:
		return 75.f;
		break;

	case ProjectJ::MatchPlayerState::ALIVE_CRITICAL:
		return 50.f;
		break;

	case ProjectJ::MatchPlayerState::ALIVE_MORIBUND:
		return 25.f;
		break;
	default:
		return 0.f;
		break;
	}
}

bool UJInGameUserWidget::IsTickEnd(uint64 input)
{
	if (input >= EndTick)
	{
		return true;
	}
	return false;
}


void UJInGameUserWidget::OnUpdateUIInfo()
{
	//시간 업데이트
	if (!IsTickEnd(CurrentTick))
	{
		CurrentTick += 1000;

		Time_m = ((EndTick - CurrentTick) / 1000) / 60;
		Time_s = ((EndTick - CurrentTick) / 1000) % 60;

		TimeBlock->SetText(FText::FromString(FString::FromInt(Time_m) + ":" + FString::FromInt(Time_s)));
	}

	//체력 업데이트

	if (GetWorld()->GetTimerManager().IsTimerActive(Timer))
	{
		Fug1HpBar->SetPercent(CalHp(LocalCurrentState->CurrentState));
		Fug2HpBar->SetPercent(CalHp(LocalCurrentState->CurrentState));
		Fug3HpBar->SetPercent(CalHp(LocalCurrentState->CurrentState));
	}
}
