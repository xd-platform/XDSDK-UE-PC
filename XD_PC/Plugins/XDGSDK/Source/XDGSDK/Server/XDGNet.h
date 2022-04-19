#pragma once
#include "TDSHttpManager.h"
#include "IpInfoModel.h"
#include "XDGError.h"

class XDGNet: public TDSHttpRequest
{
public:

	XDGNet();

	static void RequestConfig();

	// static void RequestIpInfo(void(*callback)(bool success, TSharedPtr<FIpInfoModel> model));
	// template<typename FunctorType, typename... VarTypes>

	static void RequestIpInfo(TFunction<void(TSharedPtr<FIpInfoModel> model, FXDGError error)> callback);

private:
	virtual TMap<FString, FString> CommonHeaders() override;
	virtual TSharedPtr<FJsonObject> CommonParameters() override;

	FString GetMacToken();
};
