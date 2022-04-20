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
	// g_InitState = InitStateIniting;
	// XDGNet::RequestConfig();
	XDGImplement::GetIpInfo(
	[=] (TSharedPtr<FIpInfoModel> model, FString msg)
	{
		if (model == nullptr)
		{
			// g_InitState = InitStateUninit;
			XDG_LOG(Warning, TEXT("No Model !!!"));
			GetXDGSDKEventDispatcher()->OnInitSDK.Broadcast(false, msg);
		} else
		{
			XDGImplement::InitSDK(sdkClientId,
				[](bool successed, FString msg)
				{
					
				}
			);
			XDG_LOG(Warning, TEXT("%s, %s, %s, %s, %s"), *model->city, *model->country, *model->country_code, *model->latitude, *model->longitude);
		}
	}
	);
}

void UXDGAPI::SetLanguage(LangType type)
{
	LanguageManager::SetLanguageType(type);
}


