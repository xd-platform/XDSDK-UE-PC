// Copyright Epic Games, Inc. All Rights Reserved.


#include "XD_PCGameModeBase.h"

#include "JsonHelper.h"
#include "TapMomentApi.h"
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

void AXD_PCGameModeBase::XDLogout()
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

void AXD_PCGameModeBase::CheckPay()
{
	UXDGAPI::CheckPay([](CheckPayType CheckType)
	{
		switch (CheckType)
		{
		case iOSAndAndroid:
			TDUDebuger::DisplayShow(TEXT("iOSAndAndroid"));
			break;
		case iOS:
			TDUDebuger::DisplayShow(TEXT("iOS"));
			break;
		case Android:
			TDUDebuger::DisplayShow(TEXT("Android"));
			break;
		case None:
			TDUDebuger::DisplayShow(TEXT("None"));
			break;
		}
	}, [](FXDGError Error)
	{
		TDUDebuger::DisplayShow(Error.msg);
	});
}

void AXD_PCGameModeBase::OpenCustomerCenter() {
	UXDGAPI::OpenCustomerCenter("serverId", "roleId", "roleName");
}

void AXD_PCGameModeBase::OpenWebPay(FString ServerId, FString RoleId) {
	UXDGAPI::OpenWebPay(ServerId, RoleId);
}

void AXD_PCGameModeBase::IsPushServiceEnable() {
	if (UXDGAPI::IsPushServiceEnable()) {
		TDUDebuger::DisplayShow("Push Service Enable");
	} else {
		TDUDebuger::DisplayShow("Push Service Disable");
	}
}

void AXD_PCGameModeBase::OpenWebTopic(FString AppId) {
	// TAPMOMENT_API
	TapMomentConfig Config;
	Config.AppId = AppId;
	Config.IsCN = false;
	TapMomentApi::Init(Config);
	TapMomentApi::OpenWebTopic();
}
