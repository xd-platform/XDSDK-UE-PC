// Fill out your copyright notice in the Description page of Project Settings.


#include "XDGAPI.h"
#include "XDGImplement.h"
#include "LanguageManager.h"
#include "TapLoginHelper.h"
#include "TDSHelper.h"
#include "TDUDebuger.h"
#include "XDGSDK.h"
#include "Blueprint/UserWidget.h"
#include "TapLogin/UI/TAULoginWidget.h"
#include "XDGSDK/UI/XDGPrivacyWidget.h"
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


void UXDGAPI::InitSDK(const FString& ClientId, TFunction<void(bool Result, FString Message)> CallBack)
{
	if (g_InitState == InitStateIniting)
	{
		return;
	}
	if (g_InitState == InitStateInited)
	{
		if (CallBack)
		{
			CallBack(true, TEXT("已经初始化"));
		}
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
			if (CallBack)
			{
				CallBack(false, msg);
			}
		} else
		{
			XDGImplement::InitSDK(ClientId,
				[=](bool successed, FString InitMsg)
				{
					if (successed)
					{
						g_InitState = InitStateInited;
						XDG_LOG(Display, TEXT("init success"));
						if (CallBack)
						{
							CallBack(true, InitMsg);
						}
					} else
					{
						g_InitState = InitStateUninit;
						XDG_LOG(Warning, TEXT("init fail"));
						if (CallBack)
						{
							CallBack(false, InitMsg);
						}
					}
				}
			);
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


void UXDGAPI::OpenUserCenter()
{
	if (!FXDGUser::GetLocalModel().IsValid()) {
		TDUDebuger::WarningLog("Please Login First");
		return;
	}

	UXDGUserCenterWidget::ShowWidget();
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

void UXDGAPI::Test()
{
	FString temp = TEXT("<b>这个<b>不<b>厉害么</b>哦哦</b>真逗。</b><b>这样不太好。</b><b>这个是真<b>的个</b>厉害</b>你行不行<b>不要把</b>后</b>四位3ew<b>后四位<b><b>");
	int index = 0;
	TArray<int> HeadTags;
	while (true) {
		index = temp.Find(TEXT("<b>"), ESearchCase::CaseSensitive, ESearchDir::FromStart, index);
		if (index == INDEX_NONE) {
			break;
		}
		HeadTags.Add(index);
		TDUDebuger::DisplayLog(FString::Printf(TEXT("%d"), index));
		index += 3;
	}
	index = 0;
	TArray<int> TailTags;
	while (true) {
		index = temp.Find(TEXT("</b>"), ESearchCase::CaseSensitive, ESearchDir::FromStart, index);
		if (index == INDEX_NONE) {
			break;
		}
		TailTags.Add(index);
		TDUDebuger::DisplayLog(FString::Printf(TEXT("%d"), index));
		index += 4;
	}
	int HeadIndex = 0;
	int TailIndex = 0;
	TArray<TArray<int>> Duis;
	TArray<TArray<int>> DuiStack;
	TMap<int, int> NeedDelete;
	while (!(HeadIndex == HeadTags.Num() && TailIndex == TailTags.Num())) {
		bool DealHead = false;
		if (HeadIndex == HeadTags.Num()) {
			DealHead = false;
		} else if (TailIndex == TailTags.Num()) {
			DealHead = true;
		} else {
			if (HeadTags[HeadIndex] < TailTags[TailIndex]) {
				DealHead = true;
			} else {
				DealHead = false;
			}
		}
		if (DealHead) {
			TArray<int> Dui;
			Dui.Add(HeadTags[HeadIndex]);
			DuiStack.Add(Dui);
			HeadIndex++;
		} else {
			if (DuiStack.Num() == 0) {
				NeedDelete.Add(TailTags[TailIndex], 4);
			} else {
				TArray<int> Dui = DuiStack.Last();
				Dui.Add(TailTags[TailIndex]);
				Duis.Add(Dui);
				DuiStack.RemoveAt(DuiStack.Num()-1);
			}
			TailIndex++;
		}
	}
	for (auto Stack : DuiStack) {
		for (auto TempIndex : Stack) {
			NeedDelete.Add(TempIndex, 3);
		}
	}
	for (int i = 0; i < Duis.Num() - 1; i++) {
		TArray<int> FirstDui = Duis[i];
		TArray<int> SecondDui = Duis[i+1];
		if (FirstDui[0] > SecondDui[0]) {
			NeedDelete.Add(FirstDui[0], 3);
			NeedDelete.Add(FirstDui[1], 4);
		}
	}
	NeedDelete.KeySort([](int First, int Second) {
		return First > Second;
	});
	TDUDebuger::DisplayLog("-------------");
	TDUDebuger::DisplayLog(temp);
	for (auto Delete : NeedDelete) {
		TDUDebuger::DisplayLog(FString::Printf(TEXT("%d"), Delete.Key));
		temp.RemoveAt(Delete.Key, Delete.Value, false);
	}
	TDUDebuger::DisplayLog(temp);

	
}

void UXDGAPI::ResetPrivacy()
{
	DataStorage<FXDGStorage>::Remove(FXDGStorage::PrivacyKey);
}

void UXDGAPI::OpenPayHintAlert()
{
	UXDIPayHintAlert::Show(iOSAndAndroid);
}


