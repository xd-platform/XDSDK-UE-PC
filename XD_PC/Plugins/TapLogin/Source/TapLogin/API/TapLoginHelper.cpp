// Fill out your copyright notice in the Description page of Project Settings.


#include "TapLoginHelper.h"

#include "TapLoginImpl.h"


void UTapLoginHelper::Init(const FString& clientID, bool isCn, bool roundCorner)
{
	TapLoginImpl::Get().Init(clientID, isCn, roundCorner);
}

void UTapLoginHelper::Login(TFunction<void(TUAuthResult Result)> CallBack)
{
	TArray<FString> Permissions;
	Permissions.Add(TapLoginPermissionScope::Profile);
	Login(Permissions, CallBack);
}

void UTapLoginHelper::Login(TArray<FString> Permissions, TFunction<void(TUAuthResult Result)> CallBack)
{
	TapLoginImpl::Get().Login(Permissions, CallBack);
}

void UTapLoginHelper::Logout()
{
	FTULoginProfileModel::ClearLocalModel();
	FTUAccessToken::ClearLocalModel();
}

TSharedPtr<FTULoginProfileModel> UTapLoginHelper::GetProfile()
{
	return FTULoginProfileModel::GetLocalModel();
}

TSharedPtr<FTUAccessToken> UTapLoginHelper::GetAccessToken()
{
	return FTUAccessToken::GetLocalModel();
}
