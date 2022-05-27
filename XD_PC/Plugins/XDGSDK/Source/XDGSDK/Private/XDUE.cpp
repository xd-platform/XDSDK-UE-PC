#include "XDUE.h"

#include "LanguageManager.h"
#include "TapUELogin.h"
#include "TUDebuger.h"
#include "XDGImplement.h"
#include "XDGSDK/UI/XDGUserCenterWidget.h"
#include "XDGSDK/UI/XDIPayHintAlert.h"

enum InitState
{
	InitStateUninit,
	InitStateIniting,
	InitStateInited,
};
	
static InitState g_InitState = InitStateUninit;

void XDUE::InitSDK(const XUType::Config& Config, TFunction<void(bool Result, const FString& Message)> CallBack) {
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
			TUDebuger::WarningLog("No IpInfo Model");
			if (CallBack) {
				CallBack(false, msg);
			}
		}
		else {
			XDGImplement::InitSDK(Config.ClientId, [=](bool successed, FString InitMsg) {
				if (successed) {
					g_InitState = InitStateInited;
					TUDebuger::WarningLog("No IpInfo Model");
					if (CallBack) {
						CallBack(true, InitMsg);
					}
				}
				else {
					g_InitState = InitStateUninit;
					TUDebuger::WarningLog("init fail");
					if (CallBack) {
						CallBack(false, InitMsg);
					}
				}
			});
		}
	});
}

void XDUE::LoginByType(XUType::LoginType Type, TFunction<void(const FXDGUser& User)> SuccessBlock,
	TFunction<void(const FXDGError& Error)> FailBlock) {
	if (!IsInitialized())
	{
		if (FailBlock)
		{
			FailBlock(FXDGError("Please init first"));
		}
		return;
	}

	XDGImplement::LoginByType(Type,
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

bool XDUE::IsInitialized() {
	return g_InitState == InitStateInited;;
}

void XDUE::SetLanguage(XUType::LangType Type) {
	LanguageManager::SetLanguageType(Type);
}

void XDUE::Logout() {
	// await TDSUser.Logout();
	TapUELogin::Logout();
	FXDGUser::ClearUserData();
}

void XDUE::OpenUserCenter(TFunction<void(XUType::LoginType Type, TSharedPtr<FXDGError>)> BindCallBack,
	TFunction<void(XUType::LoginType Type, TSharedPtr<FXDGError>)> UnbindCallBack) {
	if (!FXDGUser::GetLocalModel().IsValid()) {
		TUDebuger::WarningLog("Please Login First");
		return;
	}

	UXDGUserCenterWidget::ShowWidget(BindCallBack, UnbindCallBack);
}

void XDUE::CheckPay(TFunction<void(XUType::CheckPayType CheckType)> SuccessBlock,
	TFunction<void(const FXDGError& Error)> FailBlock) {
	if (!FXDGUser::GetLocalModel().IsValid()) {
		if (FailBlock)
		{
			FailBlock(FXDGError("Please Login First"));
		}
		return;
	}
	XDGImplement::CheckPay([=](XUType::CheckPayType CheckType)
	{
		if (CheckType != XUType::None)
		{
			UXDIPayHintAlert::Show(CheckType);
		}
		if (SuccessBlock)
		{
			SuccessBlock(CheckType);
		}
	}, FailBlock);
}

void XDUE::OpenCustomerCenter(const FString& ServerId, const FString& RoleId, const FString& RoleName) {
	FString UrlStr = XDGImplement::GetCustomerCenter(ServerId, RoleId, RoleName);

	if (UrlStr.IsEmpty()) {
		TUDebuger::ErrorLog("please login first");
	} else {
		FPlatformProcess::LaunchURL(*UrlStr, nullptr, nullptr);
	}
}

void XDUE::OpenWebPay(const FString& ServerId, const FString& RoleId) {
	FString UrlStr = XDGImplement::GetPayUrl(ServerId, RoleId);

	if (UrlStr.IsEmpty()) {
		TUDebuger::ErrorLog("please login first");
	} else {
		TUDebuger::DisplayLog(FString::Printf(TEXT("web pay url: %s"), *UrlStr));
		FPlatformProcess::LaunchURL(*UrlStr, nullptr, nullptr);
	}
}

void XDUE::SetPushServiceEnable(bool enable) {
	FXDGUser::SetPushServiceEnable(enable);
}

bool XDUE::IsPushServiceEnable() {
	return FXDGUser::IsPushServiceEnable();
}

#if !UE_BUILD_SHIPPING
// only test

void XDUE::Test() {
}

void XDUE::ResetPrivacy() {
	TUDataStorage<FXDGStorage>::Remove(FXDGStorage::PrivacyKey);
}

void XDUE::OpenPayHintAlert() {
	UXDIPayHintAlert::Show(XUType::iOSAndAndroid);
}

#endif


