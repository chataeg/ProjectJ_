// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/JHttpObject.h"

void UJHttpObject::PostLoginRequest(const FString& Name, const FString& Password)
{
	if (Address.IsEmpty())
	{
		return;
	}

	FHttpRequestRef RequestPtr = FHttpModule::Get().CreateRequest();
	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("name", *Name);
	RequestObj->SetStringField("password", *Password);

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);

	RequestPtr->OnProcessRequestComplete().BindUObject(this, &UJHttpObject::ProcessPost);
	RequestPtr->SetURL("http://" + Address + "/api/users/login");
	RequestPtr->SetVerb("POST");
	RequestPtr->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	RequestPtr->SetContentAsString(RequestBody);
	RequestPtr->ProcessRequest();
}

void UJHttpObject::PostCreateRequest(const FString& Name, const FString& Nickname, const FString& Password)
{
	if (Address.IsEmpty())
	{
		return;
	}

	FHttpRequestRef RequestPtr = FHttpModule::Get().CreateRequest();
	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("name", *Name);
	RequestObj->SetStringField("nickname", *Nickname);
	RequestObj->SetStringField("password", *Password);

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);

	RequestPtr->OnProcessRequestComplete().BindUObject(this, &UJHttpObject::ProcessPost);
	RequestPtr->SetURL("http://" + Address + "/api/users/create");
	RequestPtr->SetVerb("POST");
	RequestPtr->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	RequestPtr->SetContentAsString(RequestBody);
	RequestPtr->ProcessRequest();
}

void UJHttpObject::ProcessPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully == false || Response.IsValid() == false)
	{
		UE_LOG(LogTemp, Error, TEXT("HTTP Request Error"));

		if (HttpResponseInvalidDelegate.IsBound())
		{
			HttpResponseInvalidDelegate.Execute();
		}

		return;
	}

	int32 StatusCode = Response->GetResponseCode();
	FString ResponseString = Response->GetContentAsString();

	switch (StatusCode)
	{
	case EHttpResponseCodes::Ok: // Login
		{
			if (HttpResponseOkDelegate.IsBound())
			{
				HttpResponseOkDelegate.Broadcast(Request, Response);
			}
			break;
		}
	case EHttpResponseCodes::Created: // Register
		{
			HttpResponseCreatedDelegate.ExecuteIfBound(Request, Response);
			break;
		}
	case EHttpResponseCodes::BadRequest: // Register
		{
			HttpResponseBadRequestDelegate.ExecuteIfBound(Request, Response);
			break;
		}
	case EHttpResponseCodes::Denied: // Login
		{
			HttpResponseDeniedDelegate.ExecuteIfBound(Request, Response);
			break;
		}
	case EHttpResponseCodes::Conflict: // Register
		{
			HttpResponseConflictDelegate.ExecuteIfBound(Request, Response);
			break;
		}
	default:
		{
			HttpResponseInvalidDelegate.ExecuteIfBound();
			break;
		}
	}
}
