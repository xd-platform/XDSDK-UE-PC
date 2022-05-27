// Copyright Epic Games, Inc. All Rights Reserved.


#include "XD_PCGameModeBase.h"

#include "TUJsonHelper.h"
#include "TapMomentApi.h"
#include "TUDebuger.h"
#include "XDUE.h"


void AXD_PCGameModeBase::InitSDK(const FString& ClientId)
{
	XUType::Config Config;
	Config.ClientId = ClientId;
	XDUE::InitSDK(Config, [](bool Result, FString Message)
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

void AXD_PCGameModeBase::LoginByType(int LoginType)
{
	XDUE::LoginByType((XUType::LoginType)LoginType, [](FXUUser User)
	{
		TUDebuger::DisplayShow(TEXT("登录成功：") + TUJsonHelper::GetJsonString(User));
	}, [](FXUError Error)
	{
		TUDebuger::WarningShow(TEXT("登录失败：") + Error.msg);
	});
}

bool AXD_PCGameModeBase::IsInitialized()
{
	return XDUE::IsInitialized();
}

void AXD_PCGameModeBase::SetLanguage(int LangType)
{
	XDUE::SetLanguage((XUType::LangType)LangType);
}

void AXD_PCGameModeBase::ResetPrivacy()
{
	XDUE::ResetPrivacy();
	TUDebuger::DisplayShow(TEXT("重置隐私"));
}

void AXD_PCGameModeBase::XDLogout()
{
	XDUE::Logout();
	TUDebuger::DisplayShow(TEXT("退出登录"));
}

void AXD_PCGameModeBase::Test()
{
	XDUE::Test();
}

void AXD_PCGameModeBase::OpenUserCenter() {
	XDUE::OpenUserCenter(
		[](XUType::LoginType Type, TSharedPtr<FXUError> Error) {
			if (Error.IsValid()) {
				TUDebuger::DisplayShow( FString::Printf(TEXT("绑定失败, Error: %s"), *Error->msg));
			}
			else {
				TUDebuger::DisplayShow( FString::Printf(TEXT("绑定成功, Type: %d"), Type));
			}
		},
		[](XUType::LoginType Type, TSharedPtr<FXUError> Error) {
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
	XDUE::OpenPayHintAlert();
}

void AXD_PCGameModeBase::CheckPay()
{
	XDUE::CheckPay([](XUType::CheckPayType CheckType)
	{
		switch (CheckType)
		{
		case XUType::iOSAndAndroid:
			TUDebuger::DisplayShow(TEXT("iOSAndAndroid"));
			break;
		case XUType::iOS:
			TUDebuger::DisplayShow(TEXT("iOS"));
			break;
		case XUType::Android:
			TUDebuger::DisplayShow(TEXT("Android"));
			break;
		case XUType::None:
			TUDebuger::DisplayShow(TEXT("None"));
			break;
		}
	}, [](FXUError Error)
	{
		TUDebuger::DisplayShow(Error.msg);
	});
}

void AXD_PCGameModeBase::OpenCustomerCenter() {
	XDUE::OpenCustomerCenter("serverId", "roleId", "roleName");
}

void AXD_PCGameModeBase::OpenWebPay(FString ServerId, FString RoleId) {
	XDUE::OpenWebPay(ServerId, RoleId);
}

void AXD_PCGameModeBase::IsPushServiceEnable() {
	if (XDUE::IsPushServiceEnable()) {
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
