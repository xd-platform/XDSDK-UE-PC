// Fill out your copyright notice in the Description page of Project Settings.


#include "XDGAPI.h"
#include "Server/XDGNet.h"
#include "Tools/DataStorageName.h"

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

void GetIpInfo(TFunction<void(TSharedPtr<FIpInfoModel> model, FString msg)> resultBlock)
{
	XDGNet::RequestIpInfo(
		[=] (TSharedPtr<FIpInfoModel> model, FXDGError error)
		{
			if (model == nullptr)
			{
				TSharedPtr<FIpInfoModel> infoModel = DataStorage::LoadStruct<FIpInfoModel>(DataStorageName_IpInfo);
				if (resultBlock) { resultBlock(infoModel, error.msg);}
			} else
			{
				DataStorage::SaveStruct(DataStorageName_IpInfo, model, true);
				if (resultBlock) { resultBlock(model, "success");}
			}
		}
	);
}

void UXDGAPI::InitSDK(FString sdkClientId)
{
	UE_LOG(XDGSDKLog, Display, TEXT("初始化Client ID：%s"), *sdkClientId);
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
	GetIpInfo(
	[=] (TSharedPtr<FIpInfoModel> model, FString msg)
	{
		if (model == nullptr)
		{
			// g_InitState = InitStateUninit;
			UE_LOG(LogTemp, Warning, TEXT("No Model !!!"));
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
