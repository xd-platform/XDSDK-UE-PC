#pragma once
#include "TapAccessToken.h"
#include "XDGEnumType.h"
#include "XDGNet.h"
#include "XDGUser.h"

class XDGImplement
{
public:

	static void GetIpInfo(TFunction<void(TSharedPtr<FIpInfoModel> model, FString msg)> resultBlock);

	static void InitSDK(FString sdkClientId, TFunction<void(bool successed, FString msg)> resultBlock);

	static void LoginByType(LoginType loginType, TFunction<void(TSharedPtr<FXDGUser> user)> resultBlock, TFunction<void(FXDGError error)> ErrorBlock);

private:

	static void InitBootstrap(const TSharedPtr<FInitConfigModel>& model, TFunction<void(bool successed, FString msg)> resultBlock, const FString& msg);

	static void GetLoginParam(LoginType loginType, TFunction<void(TSharedPtr<FJsonObject> paras)> resultBlock, TFunction<void(FXDGError error)> ErrorBlock);

	static void RequestKidToken(TSharedPtr<FJsonObject> paras, TFunction<void(TSharedPtr<FTokenModel> kidToken)> resultBlock, TFunction<void(FXDGError error)> ErrorBlock);

	static void RequestUserInfo(bool saveToLocal, TFunction<void(TSharedPtr<FXDGUser> model)> callback, TFunction<void(FXDGError error)> ErrorBlock);

	static void AsyncNetworkTdsUser(const FString& userId, TFunction<void(FString SessionToken)> callback, TFunction<void(FXDGError error)> ErrorBlock);

	static void AsyncLocalTdsUser(const FString& userId, const FString& sessionToken);
	
	static void CheckPrivacyAlert(TFunction<void()> Callback);
	
	static void RequestTapToken(TFunction<void(FTapAccessToken AccessToken)> callback, TFunction<void(FXDGError error)> ErrorBlock);
	
};
