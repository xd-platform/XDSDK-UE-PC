// Copyright Epic Games, Inc. All Rights Reserved.


#include "XD_PCGameModeBase.h"

#include "JsonHelper.h"
#include "TDUDebuger.h"


void AXD_PCGameModeBase::InitSDK(const FString& ClientId)
{
	UXDGAPI::InitSDK(ClientId, [](bool Result, FString Message)
	{
		if (Result)
		{
			TDUDebuger::DisplayShow(Message);
		} else
		{
			TDUDebuger::WarningShow(Message);
		}
	});
}

void AXD_PCGameModeBase::LoginByType(LoginType LoginType)
{
	UXDGAPI::LoginByType(LoginType, [](FXDGUser User)
	{
		TDUDebuger::DisplayShow(TEXT("登录成功：") + JsonHelper::GetJsonString(User));
	}, [](FXDGError Error)
	{
		TDUDebuger::WarningShow(TEXT("登录失败：") + Error.msg);
	});
}

bool AXD_PCGameModeBase::IsInitialized()
{
	return UXDGAPI::IsInitialized();
}

void AXD_PCGameModeBase::SetLanguage(LangType type)
{
	UXDGAPI::SetLanguage(type);
}

void AXD_PCGameModeBase::ResetPrivacy()
{
	UXDGAPI::ResetPrivacy();
	TDUDebuger::DisplayShow(TEXT("重置隐私"));
}

void AXD_PCGameModeBase::Logout()
{
	UXDGAPI::Logout();
	TDUDebuger::DisplayShow(TEXT("退出登录"));
}

void AXD_PCGameModeBase::Test()
{
	UXDGAPI::Test();
}

void AXD_PCGameModeBase::OpenUserCenter()
{
	UXDGAPI::OpenUserCenter();
}

void AXD_PCGameModeBase::OpenPayHintAlert()
{
	UXDGAPI::OpenPayHintAlert();
}
