#pragma once
#include "TAULoginError.h"
#include "TAUQrCodeModel.h"
#include "TDUHttpRequest.h"


class TAULoginNet: public TDUHttpRequest
{
public:

	TAULoginNet();
	

	static void RequestLoginQrCode(const TArray<FString> Permissions, TFunction<void(TSharedPtr<FTAUQrCodeModel> Model, FTAULoginError Error)> callback);




private:
	virtual TMap<FString, FString> CommonHeaders() override;
	virtual TSharedPtr<FJsonObject> CommonParameters() override;
	virtual bool ResetHeadersBeforeRequest() override;
	// virtual void DoSomeingAfterCombinHeadersAndParas() override;

	// FString GetMacToken();
};
