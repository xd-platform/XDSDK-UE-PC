#pragma once
#include "TUAccessToken.h"

#include "XUNet.h"
#include "XUUser.h"

class XUImpl
{
public:

	void GetIpInfo(TFunction<void(TSharedPtr<FXUIpInfoModel> model, FString msg)> resultBlock);

	void Init(TFunction<void(bool Result, const FString& Message)> CallBack);
	
	void InitSDK(FString sdkClientId, TFunction<void(bool successed, FString msg)> resultBlock);

	void LoginByType(XUType::LoginType LoginType, TFunction<void(TSharedPtr<FXUUser> user)> resultBlock, TFunction<void(FXUError error)> ErrorBlock);

	void GetLoginParam(XUType::LoginType LoginType, TFunction<void(TSharedPtr<FJsonObject> paras)> resultBlock, TFunction<void(FXUError error)> ErrorBlock);

	void CheckPay(TFunction<void(XUType::CheckPayType CheckType)> SuccessBlock, TFunction<void(const FXUError& Error)> FailBlock);

	FString GetCustomerCenter(const FString& ServerId, const FString& RoleId, const FString& RoleName);

	FString GetPayUrl(const FString& ServerId, const FString& RoleId);

	FString GetPayUrl(const FString& ServerId, const FString& RoleId, const FString& OrderId, const FString& ProductId,
	const FString& ProductName, float PayAmount, const FString& Ext);
	
	void ResetPrivacy();
	
	static TSharedPtr<XUImpl>& Get();

	XUType::Config Config;


private:
	static TSharedPtr<XUImpl> Instance;

	static void InitBootstrap(const TSharedPtr<FXUServerConfig>& model, TFunction<void(bool successed, FString msg)> resultBlock, const FString& msg);
	
	static void RequestKidToken(TSharedPtr<FJsonObject> paras, TFunction<void(TSharedPtr<FXUTokenModel> kidToken)> resultBlock, TFunction<void(FXUError error)> ErrorBlock);

	static void RequestUserInfo(bool saveToLocal, TFunction<void(TSharedPtr<FXUUser> model)> callback, TFunction<void(FXUError error)> ErrorBlock);

	static void AsyncNetworkTdsUser(const FString& userId, TFunction<void(FString SessionToken)> callback, TFunction<void(FXUError error)> ErrorBlock);

	static void AsyncLocalTdsUser(const FString& userId, const FString& sessionToken);
	
	static void CheckPrivacyAlert(TFunction<void()> Callback);
	
	static void RequestTapToken(TFunction<void(FTUAccessToken AccessToken)> callback, TFunction<void(FXUError error)> ErrorBlock);
	
};
