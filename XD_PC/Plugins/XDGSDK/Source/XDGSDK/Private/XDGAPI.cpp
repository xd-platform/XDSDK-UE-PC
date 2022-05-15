// Fill out your copyright notice in the Description page of Project Settings.


#include "XDGAPI.h"
#include "XDGImplement.h"
#include "LanguageManager.h"
#include "TapLoginHelper.h"
#include "TDUDebuger.h"
#include "XDGSDK.h"
#include "Blueprint/UserWidget.h"
#include "XDGSDK/UI/XDGUserCenterWidget.h"
#include "XDGSDK/UI/XDIPayHintAlert.h"


enum InitState
{
	InitStateUninit,
	InitStateIniting,
	InitStateInited,
};
	
static InitState g_InitState = InitStateUninit;
// static const UXDGAPI* XDGSDKManager = nullptr;
//
// 	
// const UXDGAPI*& UXDGAPI::GetXDGSDKEventDispatcher()
// {
// 	if (XDGSDKManager == nullptr)
// 	{
// 		XDGSDKManager = NewObject<UXDGAPI>();
// 	}
// 	return XDGSDKManager;
// }


void UXDGAPI::InitSDK(const FString& ClientId, TFunction<void(bool Result, FString Message)> CallBack) {
	if (g_InitState == InitStateIniting) {
		return;
	}
	if (g_InitState == InitStateInited) {
		if (CallBack) {
			CallBack(true, TEXT("已经初始化"));
		}
		return;
	}
	g_InitState = InitStateIniting;
	XDGImplement::GetIpInfo([=](TSharedPtr<FIpInfoModel> model, FString msg) {
		if (model == nullptr) {
			g_InitState = InitStateUninit;
			XDG_LOG(Warning, TEXT("No IpInfo Model"));
			if (CallBack) {
				CallBack(false, msg);
			}
		}
		else {
			XDGImplement::InitSDK(ClientId, [=](bool successed, FString InitMsg) {
				if (successed) {
					g_InitState = InitStateInited;
					XDG_LOG(Display, TEXT("init success"));
					if (CallBack) {
						CallBack(true, InitMsg);
					}
				}
				else {
					g_InitState = InitStateUninit;
					XDG_LOG(Warning, TEXT("init fail"));
					if (CallBack) {
						CallBack(false, InitMsg);
					}
				}
			});
		}
	});
}

void UXDGAPI::LoginByType(LoginType LoginType, TFunction<void(FXDGUser User)> SuccessBlock,
	TFunction<void(FXDGError Error)> FailBlock)
{
	if (!IsInitialized())
	{
		if (FailBlock)
		{
			FailBlock(FXDGError("Please init first"));
		}
		return;
	}

	XDGImplement::LoginByType(LoginType,
	[=](TSharedPtr<FXDGUser> user)
	{
		if (SuccessBlock)
		{
			SuccessBlock(*user.Get());
		}
	},
	[=](FXDGError error)
	{
		if (FailBlock)
		{
			FailBlock(error);
		}
	});
}

void UXDGAPI::SetLanguage(LangType type)
{
	LanguageManager::SetLanguageType(type);
}

bool UXDGAPI::IsInitialized()
{
	return g_InitState == InitStateInited;;
}


void UXDGAPI::Logout()
{
	// await TDSUser.Logout();
	UTapLoginHelper::Logout();
	FXDGUser::ClearUserData();
}

void UXDGAPI::OpenUserCenter(TFunction<void(LoginType Type, TSharedPtr<FXDGError>)> BindCallBack,
	TFunction<void(LoginType Type, TSharedPtr<FXDGError>)> UnbindCallBack) {
	if (!FXDGUser::GetLocalModel().IsValid()) {
		TDUDebuger::WarningLog("Please Login First");
		return;
	}

	UXDGUserCenterWidget::ShowWidget(BindCallBack, UnbindCallBack);
}

void UXDGAPI::CheckPay(TFunction<void(CheckPayType CheckType)> SuccessBlock, TFunction<void(FXDGError Error)> FailBlock)
{
	if (!FXDGUser::GetLocalModel().IsValid()) {
		if (FailBlock)
		{
			FailBlock(FXDGError("Please Login First"));
		}
		return;
	}
	XDGImplement::CheckPay([=](CheckPayType CheckType)
	{
		if (CheckType != None)
		{
			UXDIPayHintAlert::Show(CheckType);
		}
		if (SuccessBlock)
		{
			SuccessBlock(CheckType);
		}
	}, FailBlock);

}

void UXDGAPI::OpenCustomerCenter(FString ServerId, FString RoleId, FString RoleName) {
	FString UrlStr = XDGImplement::GetCustomerCenter(ServerId, RoleId, RoleName);

	if (UrlStr.IsEmpty()) {
		TDUDebuger::ErrorLog("please login first");
	} else {
		FPlatformProcess::LaunchURL(*UrlStr, nullptr, nullptr);
	}
}

void UXDGAPI::OpenWebPay(FString ServerId, FString RoleId) {
	FString UrlStr = XDGImplement::GetPayUrl(ServerId, RoleId);

	if (UrlStr.IsEmpty()) {
		TDUDebuger::ErrorLog("please login first");
	} else {
		TDUDebuger::DisplayLog(FString::Printf(TEXT("web pay url: %s"), *UrlStr));
		FPlatformProcess::LaunchURL(*UrlStr, nullptr, nullptr);
	}
}

void UXDGAPI::SetPushServiceEnable(bool enable) {
	FXDGUser::SetPushServiceEnable(enable);
}

bool UXDGAPI::IsPushServiceEnable() {
	return FXDGUser::IsPushServiceEnable();
}

void UXDGAPI::Test()
{
	FString Path = FPlatformProcess::BaseDir();
	TDUDebuger::DisplayShow(Path);


	// FString Path = "/Users/huangyifeng/Products/Mac/XDGDemo/Product/MacNoEditor/XD_PC.app";
	// FMacPlatformProcess::LaunchFileInDefaultExternalApplication(*Path);
	// LaunchFileInDefaultExternalApplication
}

void UXDGAPI::ResetPrivacy()
{
	DataStorage<FXDGStorage>::Remove(FXDGStorage::PrivacyKey);
}

void UXDGAPI::OpenPayHintAlert()
{
	UXDIPayHintAlert::Show(iOSAndAndroid);
}


