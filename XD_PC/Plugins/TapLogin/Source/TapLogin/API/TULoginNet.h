#pragma once
#include "TapAccessToken.h"
#include "TULoginError.h"
#include "TAUProfileModel.h"
#include "TAUQrCodeModel.h"
#include "TUHttpRequest.h"


class TULoginNet: public TUHttpRequest
{
public:

	TULoginNet();

	
	

	static void RequestLoginQrCode(const TArray<FString> Permissions, TFunction<void(TSharedPtr<FTAUQrCodeModel> Model, FTULoginError Error)> callback);
	static void RequestAccessToken(const FString& DeviceCode, TFunction<void(TSharedPtr<FTapAccessToken> Model, FTULoginError Error)> callback);
	static void RequestProfile(const FTapAccessToken& AccessToken, TFunction<void(TSharedPtr<FTAUProfileModel> Model, FTULoginError Error)> callback);
	static void RequestAccessTokenFromWeb(const TSharedPtr<FJsonObject>& Paras, TFunction<void(TSharedPtr<FTapAccessToken> Model, FTULoginError Error)> callback);


	static FTULoginError GenerateErrorInfo(const TSharedPtr<TUHttpResponse>& Response);
private:
	virtual TMap<FString, FString> CommonHeaders() override;
	virtual TSharedPtr<FJsonObject> CommonParameters() override;
	virtual bool ResetHeadersBeforeRequest() override;

	TSharedPtr<FTapAccessToken> AccessToken = nullptr;
	FString GetMacToken();

	
};
