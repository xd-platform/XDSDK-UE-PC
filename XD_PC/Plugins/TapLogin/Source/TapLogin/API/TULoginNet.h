#pragma once
#include "TUAccessToken.h"
#include "TULoginError.h"
#include "TULoginProfileModel.h"
#include "TAUQrCodeModel.h"
#include "TUHttpRequest.h"


class TULoginNet: public TUHttpRequest
{
public:

	TULoginNet();

	
	

	static void RequestLoginQrCode(const TArray<FString> Permissions, TFunction<void(TSharedPtr<FTAUQrCodeModel> Model, FTULoginError Error)> callback);
	static void RequestAccessToken(const FString& DeviceCode, TFunction<void(TSharedPtr<FTUAccessToken> Model, FTULoginError Error)> callback);
	static void RequestProfile(const FTUAccessToken& AccessToken, TFunction<void(TSharedPtr<FTULoginProfileModel> Model, FTULoginError Error)> callback);
	static void RequestAccessTokenFromWeb(const TSharedPtr<FJsonObject>& Paras, TFunction<void(TSharedPtr<FTUAccessToken> Model, FTULoginError Error)> callback);


	static FTULoginError GenerateErrorInfo(const TSharedPtr<TUHttpResponse>& Response);
private:
	virtual TMap<FString, FString> CommonHeaders() override;
	virtual TSharedPtr<FJsonObject> CommonParameters() override;
	virtual bool ResetHeadersBeforeRequest() override;

	TSharedPtr<FTUAccessToken> AccessToken = nullptr;
	FString GetMacToken();

	
};
