#pragma once
#include "TUAccessToken.h"

#include "XUNet.h"
#include "XUUser.h"

typedef TFunction<void(bool Result, const FString& Message)> XUInitCallback;

class XUImpl
{
public:
	
	void InitSDK(const FString& GameVersion, XUInitCallback CallBack);
	
	void InitSDK(TSharedPtr<XUType::Config> Config, XUInitCallback CallBack);
	
	void LoginByType(XUType::LoginType LoginType, TFunction<void(TSharedPtr<FXUUser> user)> resultBlock, TFunction<void(FXUError error)> ErrorBlock);

	void GetAuthParam(XUType::LoginType LoginType, TFunction<void(TSharedPtr<FJsonObject> paras)> resultBlock, TFunction<void(FXUError error)> ErrorBlock);

	void CheckPay(TFunction<void(XUType::CheckPayType CheckType)> SuccessBlock, TFunction<void(const FXUError& Error)> FailBlock);

	FString GetCustomerCenter(const FString& ServerId, const FString& RoleId, const FString& RoleName);

	static void OpenWebPay(const FString& ServerId,
						   const FString& RoleId,
						   const FString& ProductId,
						   const FString& OrderId,
						   const FString& ProductName = "",
						   float PayAmount = 0,
						   const FString& Ext = "",
						   TFunction<void(XUType::PayResult Result)> CallBack = nullptr);
	
	void ResetPrivacy();

	void AccountCancellation();
	
	static TSharedPtr<XUImpl>& Get();


private:
	static TSharedPtr<XUImpl> Instance;
	
	void RequestKidToken(TSharedPtr<FJsonObject> paras, TFunction<void(TSharedPtr<FXUTokenModel> kidToken)> resultBlock, TFunction<void(FXUError error)> ErrorBlock);

	void RequestUserInfo(bool saveToLocal, TFunction<void(TSharedPtr<FXUUser> model)> callback, TFunction<void(FXUError error)> ErrorBlock);

	void AsyncNetworkTdsUser(const FString& userId, TFunction<void(FString SessionToken)> callback, TFunction<void(FXUError error)> ErrorBlock);

	void AsyncLocalTdsUser(const FString& userId, const FString& sessionToken);
	
	void CheckAgreement(TSharedPtr<XUType::Config> Config, XUInitCallback CallBack);

	void InitFinish(XUInitCallback CallBack);

	void RequestServerConfig();

};
