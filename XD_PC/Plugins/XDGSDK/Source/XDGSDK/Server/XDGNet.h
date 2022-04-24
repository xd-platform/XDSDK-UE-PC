#pragma once
#include "InitConfigModel.h"
#include "TDSHttpManager.h"
#include "IpInfoModel.h"
#include "TokenModel.h"
#include "XDGError.h"
#include "XDGUser.h"

class XDGNet: public TDSHttpRequest
{
public:

	XDGNet();
	

	static void RequestIpInfo(TFunction<void(TSharedPtr<FIpInfoModel> model, FXDGError error)> callback);
	static void RequestConfig(TFunction<void(TSharedPtr<FInitConfigModel> model, FXDGError error)> callback);
	static void RequestKidToken(const TSharedPtr<FJsonObject>& paras, TFunction<void(TSharedPtr<FTokenModel> model, FXDGError error)> callback);
	static void RequestUserInfo(TFunction<void(TSharedPtr<FXDGUser> model, FXDGError error)> callback);
	static void RequestSyncToken(TFunction<void(TSharedPtr<FSyncTokenModel> model, FXDGError error)> callback);
	static void RequestPrivacyTxt(const FString& Url, TFunction<void(FString Txt)> callback);



private:
	virtual TMap<FString, FString> CommonHeaders() override;
	virtual TSharedPtr<FJsonObject> CommonParameters() override;
	virtual bool ResetHeadersBeforeRequest() override;
	// virtual void DoSomeingAfterCombinHeadersAndParas() override;

	FString GetMacToken();
};
