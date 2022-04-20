// Fill out your copyright notice in the Description page of Project Settings.


#include "XDGAPI.h"
#include "XDGImplement.h"
#include "LanguageManager.h"
#include "XDGSDK.h"


enum InitState
{
	InitStateUninit,
	InitStateIniting,
	InitStateInited,
};
	
static InitState g_InitState = InitStateUninit;
static UXDGAPI* XDGSDKManager = nullptr;

	
const UXDGAPI* UXDGAPI::GetXDGSDKEventDispatcher()
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


