#pragma once
#include "XUError.h"
#include "XUUser.h"
#include "XUType.h"

class XDGSDK_API XDUE {
public:
	
	static void InitSDK(const XUType::Config& Config, TFunction<void(bool Result, const FString& Message)> CallBack);

	static void LoginByType(XUType::LoginType Type, TFunction<void(const FXUUser& User)> SuccessBlock, TFunction<void(const FXUError& Error)> FailBlock);

	static bool IsInitialized();

	static void SetLanguage(XUType::LangType Type);
	
	static void Logout();
	
	static void OpenUserCenter(TFunction<void(XUType::LoginType Type, TSharedPtr<FXUError>)> BindCallBack,
	TFunction<void(XUType::LoginType Type, TSharedPtr<FXUError>)> UnbindCallBack);
	
	static void CheckPay(TFunction<void(XUType::CheckPayType CheckType)> SuccessBlock, TFunction<void(const FXUError& Error)> FailBlock);

	static void OpenCustomerCenter(const FString& ServerId, const FString& RoleId, const FString& RoleName);

	static void OpenWebPay(const FString& ServerId, const FString& RoleId);

	static void SetPushServiceEnable(bool enable);

	static bool IsPushServiceEnable();


#if !UE_BUILD_SHIPPING
	// only test

	static void Test();

	static void ResetPrivacy();

	static void OpenPayHintAlert();
#endif
	
};
