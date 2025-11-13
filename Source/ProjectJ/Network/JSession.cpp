// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/JSession.h"

void UJSession::SetSessionInfo(const int32& InId, const FString& InName, const FString& InNickname,
                               const FString& InToken)
{
	SessionId = InId;
	Name = InName;
	Nickname = InNickname;
	Token = InToken;
}

void UJSession::Clear()
{
	SessionId = -1;
	Name.Empty();
	Nickname.Empty();
	Token.Empty();
}
