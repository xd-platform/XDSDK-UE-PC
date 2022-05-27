#pragma once
#include "InitConfigModel.h"
#include "TUHttpManager.h"
#include "IpInfoModel.h"
#include "XUTokenModel.h"
#include "XDGBindModel.h"
#include "XDGError.h"
#include "XDGUser.h"
#include "XDIPayCheckModel.h"

class XDGNet: public TUHttpRequest
{
public:

	XDGNet();
	

	static void RequestIpInfo(TFunction<void(TSharedPtr<FIpInfoModel> Model, FXDGError Error)> Callback);
	static void RequestConfig(TFunction<void(TSharedPtr<FInitConfigModel> Model, FXDGError Error)> Callback);
	static void RequestKidToken(const TSharedPtr<FJsonObject>& Paras, TFunction<void(TSharedPtr<FXUTokenModel> Model, FXDGError Error)> Callback);
	static void RequestUserInfo(TFunction<void(TSharedPtr<FXDGUser> Model, FXDGError Error)> Callback);
	static void RequestSyncToken(TFunction<void(TSharedPtr<FSyncTokenModel> Model, FXDGError Error)> Callback);
	static void RequestPrivacyTxt(const FString& Url, TFunction<void(FString Txt)> Callback);
	static void RequestBindList(TFunction<void(TSharedPtr<FXDGBindResponseModel> Model, FXDGError Error)> Callback);
	static void Bind(const TSharedPtr<FJsonObject>& Paras, TFunction<void(TSharedPtr<FXDGResponseModel> Model, FXDGError Error)> Callback);
	static void Unbind(int LoginType, TFunction<void(TSharedPtr<FXDGResponseModel> Model, FXDGError Error)> Callback);
	static void CheckPay(TFunction<void(TSharedPtr<FXDIPayCheckResponseModel> Model, FXDGError Error)> Callback);


	static FXDGError GenerateErrorInfo(const TSharedPtr<TUHttpResponse>& response);
private:
	virtual TMap<FString, FString> CommonHeaders() override;
	virtual TSharedPtr<FJsonObject> CommonParameters() override;
	virtual bool ResetHeadersBeforeRequest() override;
	// virtual void DoSomeingAfterCombinHeadersAndParas() override;

	
	FString GetMacToken();
};
