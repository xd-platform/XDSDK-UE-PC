// Fill out your copyright notice in the Description page of Project Settings.


#include "XDGAPI.h"
#include "XDGImplement.h"
#include "LanguageManager.h"
#include "DeviceInfo.h"
#include "TDSCrypto.h"
#include "UrlParse.h"

DEFINE_LOG_CATEGORY_STATIC(XDGSDKLog, Log, All);
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
			UE_LOG(XDGSDKLog, Warning, TEXT("No Model !!!"));
			GetXDGSDKEventDispatcher()->OnInitSDK.Broadcast(false, msg);
		} else
		{
			// DataStorage::SaveStruct(DataStorageName::IpInfo, model);
			// if (resultBlock) { resultBlock(model, "success");}
			UE_LOG(LogTemp, Warning, TEXT("%s, %s, %s, %s, %s"), *model->city, *model->country, *model->country_code, *model->latitude, *model->longitude);
		}
	}
	);
}

void UXDGAPI::SetLanguage(LangType type)
{
	LanguageManager::SetLanguageType(type);
}


