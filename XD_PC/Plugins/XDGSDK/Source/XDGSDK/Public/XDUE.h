#pragma once
#include "XUError.h"
#include "XUUser.h"
#include "XUType.h"

class XDGSDK_API XDUE {
public:
	
	static void InitSDK(const XUType::Config& Config, TFunction<void(bool Result, const FString& Message)> CallBack);

	static void LoginByType(XUType::LoginType Type, TFunction<void(const FXUUser& User)> SuccessBlock, TFunction<void(const FXUError& Error)> FailBlock);

	// 使用前判断下IsValid，登录成功后才有值
	static TSharedPtr<FXUUser> GetUserInfo();
	static TSharedPtr<FXUTokenModel> GetAccessToken();

	static bool IsInitialized();

	static void SetLanguage(XUType::LangType Type);
	
	static void Logout();
	
	static void OpenUserCenter(TFunction<void(XUType::LoginType Type, TSharedPtr<FXUError>)> BindCallBack,
	TFunction<void(XUType::LoginType Type, TSharedPtr<FXUError>)> UnbindCallBack);
	
	static void CheckPay(TFunction<void(XUType::CheckPayType CheckType)> SuccessBlock, TFunction<void(const FXUError& Error)> FailBlock);

	static void OpenCustomerCenter(const FString& ServerId, const FString& RoleId, const FString& RoleName);

	// 海外支付
	static void OpenWebPay(const FString& ServerId, const FString& RoleId); 

	// 国内支付，海外调用该方法，自动会调用海外支付。
	static void OpenWebPay(const FString& ServerId, const FString& RoleId, const FString& OrderId,
		const FString& ProductId, const FString& ProductName, float PayAmount, const FString& Ext);

	static void SetPushServiceEnable(bool enable);

	static bool IsPushServiceEnable();


#if !UE_BUILD_SHIPPING
	// only test

	static void Test();

	static void ResetPrivacy();

	static void OpenPayHintAlert();
#endif
	
};
