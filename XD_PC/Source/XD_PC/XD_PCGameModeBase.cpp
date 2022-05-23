// Copyright Epic Games, Inc. All Rights Reserved.


#include "XD_PCGameModeBase.h"

#include "TUJsonHelper.h"
#include "TapMomentApi.h"
#include "TUDebuger.h"


void AXD_PCGameModeBase::InitSDK(const FString& ClientId)
{
	UXDGAPI::InitSDK(ClientId, [](bool Result, FString Message)
	{
		if (Result)
		{
			TUDebuger::DisplayShow(Message);
		} else
		{
			TUDebuger::WarningShow(Message);
		}
	});
}

void AXD_PCGameModeBase::LoginByType(XDLoginType LoginType)
{
	UXDGAPI::LoginByType(LoginType, [](FXDGUser User)
	{
		TUDebuger::DisplayShow(TEXT("登录成功：") + TUJsonHelper::GetJsonString(User));
	}, [](FXDGError Error)
	{
		TUDebuger::WarningShow(TEXT("登录失败：") + Error.msg);
	});
}

bool AXD_PCGameModeBase::IsInitialized()
{
	return UXDGAPI::IsInitialized();
}

void AXD_PCGameModeBase::SetLanguage(XDLangType type)
{
	UXDGAPI::SetLanguage(type);
}

void AXD_PCGameModeBase::ResetPrivacy()
{
	UXDGAPI::ResetPrivacy();
	TUDebuger::DisplayShow(TEXT("重置隐私"));
}

void AXD_PCGameModeBase::XDLogout()
{
	UXDGAPI::Logout();
	TUDebuger::DisplayShow(TEXT("退出登录"));
}

void AXD_PCGameModeBase::Test()
{
	UXDGAPI::Test();
}

void AXD_PCGameModeBase::OpenUserCenter() {
	UXDGAPI::OpenUserCenter(
		[](XDLoginType Type, TSharedPtr<FXDGError> Error) {
			if (Error.IsValid()) {
				TUDebuger::DisplayShow( FString::Printf(TEXT("绑定失败, Error: %s"), *Error->msg));
			}
			else {
				TUDebuger::DisplayShow( FString::Printf(TEXT("绑定成功, Type: %d"), Type));
			}
		},
		[](XDLoginType Type, TSharedPtr<FXDGError> Error) {
			if (Error.IsValid()) {
				TUDebuger::DisplayShow( FString::Printf(TEXT("解绑失败, Error: %s"), *Error->msg));
			}
			else {
				TUDebuger::DisplayShow( FString::Printf(TEXT("解绑成功, Type: %d"), Type));
			}
		}
	);
}

void AXD_PCGameModeBase::OpenPayHintAlert()
{
	UXDGAPI::OpenPayHintAlert();
}

void AXD_PCGameModeBase::CheckPay()
{
	UXDGAPI::CheckPay([](XDCheckPayType CheckType)
	{
		switch (CheckType)
		{
		case iOSAndAndroid:
			TUDebuger::DisplayShow(TEXT("iOSAndAndroid"));
			break;
		case iOS:
			TUDebuger::DisplayShow(TEXT("iOS"));
			break;
		case Android:
			TUDebuger::DisplayShow(TEXT("Android"));
			break;
		case None:
			TUDebuger::DisplayShow(TEXT("None"));
			break;
		}
	}, [](FXDGError Error)
	{
		TUDebuger::DisplayShow(Error.msg);
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
		TUDebuger::DisplayShow("Push Service Enable");
	} else {
		TUDebuger::DisplayShow("Push Service Disable");
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
