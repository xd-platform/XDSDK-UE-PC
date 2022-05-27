#include "XDUE.h"

#include "XULanguageManager.h"
#include "TapUELogin.h"
#include "TUDebuger.h"
#include "XUImpl.h"
#include "XDGSDK/UI/XUUserCenterWidget.h"
#include "XDGSDK/UI/XUPayHintAlert.h"

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
	XUImpl::Get()->Config = Config;
	SetLanguage(Config.LangType);
	XUImpl::GetIpInfo([=](TSharedPtr<FXUIpInfoModel> model, FString msg) {
		if (model == nullptr) {
			g_InitState = InitStateUninit;
			TUDebuger::WarningLog("No IpInfo Model");
			if (CallBack) {
				CallBack(false, msg);
			}
		}
		else {
			XUImpl::InitSDK(Config.ClientId, [=](bool successed, FString InitMsg) {
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

void XDUE::LoginByType(XUType::LoginType Type, TFunction<void(const FXUUser& User)> SuccessBlock,
	TFunction<void(const FXUError& Error)> FailBlock) {
	if (!IsInitialized())
	{
		if (FailBlock)
		{
			FailBlock(FXUError("Please init first"));
		}
		return;
	}

	XUImpl::LoginByType(Type,
	[=](TSharedPtr<FXUUser> user)
	{
		if (SuccessBlock)
		{
			SuccessBlock(*user.Get());
		}
	},
	[=](FXUError error)
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
	XULanguageManager::SetLanguageType(Type);
}

void XDUE::Logout() {
	// await TDSUser.Logout();
	TapUELogin::Logout();
	FXUUser::ClearUserData();
}

void XDUE::OpenUserCenter(TFunction<void(XUType::LoginType Type, TSharedPtr<FXUError>)> BindCallBack,
	TFunction<void(XUType::LoginType Type, TSharedPtr<FXUError>)> UnbindCallBack) {
	if (!FXUUser::GetLocalModel().IsValid()) {
		TUDebuger::WarningLog("Please Login First");
		return;
	}

	UXUUserCenterWidget::ShowWidget(BindCallBack, UnbindCallBack);
}

void XDUE::CheckPay(TFunction<void(XUType::CheckPayType CheckType)> SuccessBlock,
	TFunction<void(const FXUError& Error)> FailBlock) {
	if (!FXUUser::GetLocalModel().IsValid()) {
		if (FailBlock)
		{
			FailBlock(FXUError("Please Login First"));
		}
		return;
	}
	XUImpl::CheckPay([=](XUType::CheckPayType CheckType)
	{
		if (CheckType != XUType::None)
		{
			UXUPayHintAlert::Show(CheckType);
		}
		if (SuccessBlock)
		{
			SuccessBlock(CheckType);
		}
	}, FailBlock);
}

void XDUE::OpenCustomerCenter(const FString& ServerId, const FString& RoleId, const FString& RoleName) {
	FString UrlStr = XUImpl::GetCustomerCenter(ServerId, RoleId, RoleName);

	if (UrlStr.IsEmpty()) {
		TUDebuger::ErrorLog("please login first");
	} else {
		FPlatformProcess::LaunchURL(*UrlStr, nullptr, nullptr);
	}
}

void XDUE::OpenWebPay(const FString& ServerId, const FString& RoleId) {
	FString UrlStr = XUImpl::GetPayUrl(ServerId, RoleId);

	if (UrlStr.IsEmpty()) {
		TUDebuger::ErrorLog("please login first");
	} else {
		TUDebuger::DisplayLog(FString::Printf(TEXT("web pay url: %s"), *UrlStr));
		FPlatformProcess::LaunchURL(*UrlStr, nullptr, nullptr);
	}
}

void XDUE::SetPushServiceEnable(bool enable) {
	FXUUser::SetPushServiceEnable(enable);
}

bool XDUE::IsPushServiceEnable() {
	return FXUUser::IsPushServiceEnable();
}

#if !UE_BUILD_SHIPPING
// only test

void XDUE::Test() {
}

void XDUE::ResetPrivacy() {
	TUDataStorage<FXUStorage>::Remove(FXUStorage::PrivacyKey);
}

void XDUE::OpenPayHintAlert() {
	UXUPayHintAlert::Show(XUType::iOSAndAndroid);
}

#endif


