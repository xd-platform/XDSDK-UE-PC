#pragma once
#include "TUAccessToken.h"

#include "XDGNet.h"
#include "XDGUser.h"

class XDGImplement
{
public:

	static void GetIpInfo(TFunction<void(TSharedPtr<FIpInfoModel> model, FString msg)> resultBlock);

	static void InitSDK(FString sdkClientId, TFunction<void(bool successed, FString msg)> resultBlock);

	static void LoginByType(XUType::LoginType LoginType, TFunction<void(TSharedPtr<FXDGUser> user)> resultBlock, TFunction<void(FXDGError error)> ErrorBlock);

	static void GetLoginParam(XUType::LoginType LoginType, TFunction<void(TSharedPtr<FJsonObject> paras)> resultBlock, TFunction<void(FXDGError error)> ErrorBlock);

	static void CheckPay(TFunction<void(XUType::CheckPayType CheckType)> SuccessBlock, TFunction<void(const FXDGError& Error)> FailBlock);

	static FString GetCustomerCenter(const FString& ServerId, const FString& RoleId, const FString& RoleName);

	static FString GetPayUrl(const FString& ServerId, const FString& RoleId);

private:

	static void InitBootstrap(const TSharedPtr<FInitConfigModel>& model, TFunction<void(bool successed, FString msg)> resultBlock, const FString& msg);
	
	static void RequestKidToken(TSharedPtr<FJsonObject> paras, TFunction<void(TSharedPtr<FXUTokenModel> kidToken)> resultBlock, TFunction<void(FXDGError error)> ErrorBlock);

	static void RequestUserInfo(bool saveToLocal, TFunction<void(TSharedPtr<FXDGUser> model)> callback, TFunction<void(FXDGError error)> ErrorBlock);

	static void AsyncNetworkTdsUser(const FString& userId, TFunction<void(FString SessionToken)> callback, TFunction<void(FXDGError error)> ErrorBlock);

	static void AsyncLocalTdsUser(const FString& userId, const FString& sessionToken);
	
	static void CheckPrivacyAlert(TFunction<void()> Callback);
	
	static void RequestTapToken(TFunction<void(FTUAccessToken AccessToken)> callback, TFunction<void(FXDGError error)> ErrorBlock);
	
};
