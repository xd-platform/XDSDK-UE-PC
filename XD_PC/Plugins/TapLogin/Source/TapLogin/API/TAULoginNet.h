#pragma once
#include "TapAccessToken.h"
#include "TAULoginError.h"
#include "TAUProfileModel.h"
#include "TAUQrCodeModel.h"
#include "TDUHttpRequest.h"


class TAULoginNet: public TDUHttpRequest
{
public:

	TAULoginNet();

	
	

	static void RequestLoginQrCode(const TArray<FString> Permissions, TFunction<void(TSharedPtr<FTAUQrCodeModel> Model, FTAULoginError Error)> callback);
	static void RequestAccessToken(const FString& DeviceCode, TFunction<void(TSharedPtr<FTapAccessToken> Model, FTAULoginError Error)> callback);
	static void RequestProfile(const FTapAccessToken& AccessToken, TFunction<void(TSharedPtr<FTAUProfileModel> Model, FTAULoginError Error)> callback);


	static FTAULoginError GenerateErrorInfo(const TSharedPtr<TDUHttpResponse>& Response);
private:
	virtual TMap<FString, FString> CommonHeaders() override;
	virtual TSharedPtr<FJsonObject> CommonParameters() override;
	virtual bool ResetHeadersBeforeRequest() override;

	TSharedPtr<FTapAccessToken> AccessToken = nullptr;
	FString GetMacToken();

	
};
