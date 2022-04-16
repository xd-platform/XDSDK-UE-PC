#pragma once
#include "TDSNet/TDSHttpManager.h"
#include "Model/IpInfoModel.h"

class XDGNet: public TDSHttpRequest
{
public:

	static void RequestConfig();

	// static void RequestIpInfo(void(*callback)(bool success, TSharedPtr<FIpInfoModel> model));
	// template<typename FunctorType, typename... VarTypes>

	static void RequestIpInfo(TFunction<void(bool success, TSharedPtr<FIpInfoModel> model)> callback);



};
