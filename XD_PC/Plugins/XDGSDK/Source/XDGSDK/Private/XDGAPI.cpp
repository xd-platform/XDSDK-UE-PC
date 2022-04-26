// Fill out your copyright notice in the Description page of Project Settings.


#include "XDGAPI.h"
#include "XDGImplement.h"
#include "LanguageManager.h"
#include "TDSHelper.h"
#include "XDGSDK.h"
#include "Blueprint/UserWidget.h"
#include "TapLogin/UI/TAULoginWidget.h"
#include "XDGSDK/UI/XDGPrivacyWidget.h"


enum InitState
{
	InitStateUninit,
	InitStateIniting,
	InitStateInited,
};
	
static InitState g_InitState = InitStateUninit;
static const UXDGAPI* XDGSDKManager = nullptr;

	
const UXDGAPI*& UXDGAPI::GetXDGSDKEventDispatcher()
{
	if (XDGSDKManager == nullptr)
	{
		XDGSDKManager = NewObject<UXDGAPI>();
	}
	return XDGSDKManager;
}

void UXDGAPI::InitSDK(FString sdkClientId)
{
	if (g_InitState == InitStateIniting)
	{
		return;
	}
	if (g_InitState == InitStateInited)
	{
		GetXDGSDKEventDispatcher()->OnInitSDK.Broadcast(true, TEXT("已经初始化"));
		return;
	}
	g_InitState = InitStateIniting;
	XDGImplement::GetIpInfo(
	[=] (TSharedPtr<FIpInfoModel> model, FString msg)
	{
		if (model == nullptr)
		{
			g_InitState = InitStateUninit;
			XDG_LOG(Warning, TEXT("No IpInfo Model"));
			GetXDGSDKEventDispatcher()->OnInitSDK.Broadcast(false, msg);
		} else
		{
			XDGImplement::InitSDK(sdkClientId,
				[](bool successed, FString msg)
				{
					if (successed)
					{
						g_InitState = InitStateInited;
						XDG_LOG(Display, TEXT("init success"));
						GetXDGSDKEventDispatcher()->OnInitSDK.Broadcast(true, msg);
					} else
					{
						g_InitState = InitStateUninit;
						XDG_LOG(Warning, TEXT("init fail"));
						GetXDGSDKEventDispatcher()->OnInitSDK.Broadcast(false, msg);
					}
				}
			);
		}
	}
	);
}

void UXDGAPI::SetLanguage(LangType type)
{
	LanguageManager::SetLanguageType(type);
}

bool UXDGAPI::IsInitialized()
{
	return g_InitState == InitStateInited;;
}

void UXDGAPI::LoginByType(LoginType loginType)
{
	if (!IsInitialized())
	{
		GetXDGSDKEventDispatcher()->OnLoginFail.Broadcast(FXDGError("Please init first"));
		return;
	}

	XDGImplement::LoginByType(loginType,
	[](TSharedPtr<FXDGUser> user)
	{
		GetXDGSDKEventDispatcher()->OnLoginSuccess.Broadcast(*user.Get());
	},
	[](FXDGError error)
	{
		GetXDGSDKEventDispatcher()->OnLoginFail.Broadcast(error);
	});
}

void UXDGAPI::ResetPrivacy()
{
	DataStorage::Remove(XDGDataStorageName::PrivacyKey);
}

void UXDGAPI::Test()
{
	UTAULoginWidget::ShowLoginUI(TArray<FString>(), nullptr);
	// UXDGPrivacyWidget::ShowPrivacy([](bool result){TDSHelper::Debug(TEXT("关闭了"));});
}


