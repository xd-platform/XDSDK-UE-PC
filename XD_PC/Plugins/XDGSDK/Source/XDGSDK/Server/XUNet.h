#pragma once
#include "XUInitConfigModel.h"
#include "TUHttpManager.h"
#include "XUIpInfoModel.h"
#include "XUTokenModel.h"
#include "XUBindModel.h"
#include "XUError.h"
#include "XUUser.h"
#include "XUPayCheckModel.h"

class XUNet: public TUHttpRequest
{
public:

	XUNet();
	

	static void RequestIpInfo(TFunction<void(TSharedPtr<FXUIpInfoModel> Model, FXUError Error)> Callback);
	static void RequestConfig(TFunction<void(TSharedPtr<FXUInitConfigModel> Model, FXUError Error)> Callback);
	static void RequestKidToken(const TSharedPtr<FJsonObject>& Paras, TFunction<void(TSharedPtr<FXUTokenModel> Model, FXUError Error)> Callback);
	static void RequestUserInfo(TFunction<void(TSharedPtr<FXUUser> Model, FXUError Error)> Callback);
	static void RequestSyncToken(TFunction<void(TSharedPtr<FXUSyncTokenModel> Model, FXUError Error)> Callback);
	static void RequestPrivacyTxt(const FString& Url, TFunction<void(FString Txt)> Callback);
	static void RequestBindList(TFunction<void(TSharedPtr<FXUBindResponseModel> Model, FXUError Error)> Callback);
	static void Bind(const TSharedPtr<FJsonObject>& Paras, TFunction<void(TSharedPtr<FXUResponseModel> Model, FXUError Error)> Callback);
	static void Unbind(int LoginType, TFunction<void(TSharedPtr<FXUResponseModel> Model, FXUError Error)> Callback);
	static void CheckPay(TFunction<void(TSharedPtr<FXUPayCheckResponseModel> Model, FXUError Error)> Callback);


	static FXUError GenerateErrorInfo(const TSharedPtr<TUHttpResponse>& response);
private:
	virtual TMap<FString, FString> CommonHeaders() override;
	virtual TSharedPtr<FJsonObject> CommonParameters() override;
	virtual bool ResetHeadersBeforeRequest() override;
	// virtual void DoSomeingAfterCombinHeadersAndParas() override;

	
	FString GetMacToken();
};
