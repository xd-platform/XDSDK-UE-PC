// Fill out your copyright notice in the Description page of Project Settings.


#include "TapLoginHelper.h"

#include "TapLoginImpl.h"


void UTapLoginHelper::Init(const FString& clientID, bool isCn, bool roundCorner)
{
	TapLoginImpl::Get().Init(clientID, isCn, roundCorner);
}

void UTapLoginHelper::Login(TFunction<void(TapAuthResult Result)> CallBack)
{
	TArray<FString> Permissions;
	Permissions.Add(TapLoginPermissionScope::Profile);
	Login(Permissions, CallBack);
}

void UTapLoginHelper::Login(TArray<FString> Permissions, TFunction<void(TapAuthResult Result)> CallBack)
{
	TapLoginImpl::Get().Login(Permissions, CallBack);
}
