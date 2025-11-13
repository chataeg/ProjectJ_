// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/JThread.h"

UJThread::UJThread()
{
}

UJThread::~UJThread()
{
	if (Thread != nullptr)
	{
		delete Thread;
		Thread = nullptr;
	}
}

bool UJThread::Init()
{
	StopTaskCounter.Set(0);

	return true;
}

uint32 UJThread::Run()
{
	WorkThread();
	return 0;
}

void UJThread::Stop()
{
	StopTaskCounter.Increment();
}

void UJThread::Exit()
{
}

bool UJThread::RunThread(UJSocket* InSocket, UJGameInstance* InGameInstance)
{
	if (Thread != nullptr)
	{
		Thread->Kill(false);
		delete Thread;
	}

	Socket = InSocket;
	GameInstance = InGameInstance;

	Thread = FRunnableThread::Create(this, TEXT("RecvThread"));
	return Thread != nullptr;
}

void UJThread::StopThread()
{
	if (Thread != nullptr && StopTaskCounter.GetValue() == 0)
	{
		Stop();
		Thread->WaitForCompletion();
		Thread->Kill();
		delete Thread;
		Thread = nullptr;
	}
}
