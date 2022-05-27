#pragma once
#include "TUAccessToken.h"

#include "XUNet.h"
#include "XUUser.h"

class XUImpl
{
public:

	static void GetIpInfo(TFunction<void(TSharedPtr<FXUIpInfoModel> model, FString msg)> resultBlock);

	static void InitSDK(FString sdkClientId, TFunction<void(bool successed, FString msg)> resultBlock);

	static void LoginByType(XUType::LoginType LoginType, TFunction<void(TSharedPtr<FXUUser> user)> resultBlock, TFunction<void(FXUError error)> ErrorBlock);

	static void GetLoginParam(XUType::LoginType LoginType, TFunction<void(TSharedPtr<FJsonObject> paras)> resultBlock, TFunction<void(FXUError error)> ErrorBlock);

	static void CheckPay(TFunction<void(XUType::CheckPayType CheckType)> SuccessBlock, TFunction<void(const FXUError& Error)> FailBlock);

	static FString GetCustomerCenter(const FString& ServerId, const FString& RoleId, const FString& RoleName);

	static FString GetPayUrl(const FString& ServerId, const FString& RoleId);

private:

	static void InitBootstrap(const TSharedPtr<FXUInitConfigModel>& model, TFunction<void(bool successed, FString msg)> resultBlock, const FString& msg);
	
	static void RequestKidToken(TSharedPtr<FJsonObject> paras, TFunction<void(TSharedPtr<FXUTokenModel> kidToken)> resultBlock, TFunction<void(FXUError error)> ErrorBlock);

	static void RequestUserInfo(bool saveToLocal, TFunction<void(TSharedPtr<FXUUser> model)> callback, TFunction<void(FXUError error)> ErrorBlock);

	static void AsyncNetworkTdsUser(const FString& userId, TFunction<void(FString SessionToken)> callback, TFunction<void(FXUError error)> ErrorBlock);

	static void AsyncLocalTdsUser(const FString& userId, const FString& sessionToken);
	
	static void CheckPrivacyAlert(TFunction<void()> Callback);
	
	static void RequestTapToken(TFunction<void(FTUAccessToken AccessToken)> callback, TFunction<void(FXUError error)> ErrorBlock);
	
};
