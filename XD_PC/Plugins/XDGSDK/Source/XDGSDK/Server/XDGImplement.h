#pragma once
#include "XDGNet.h"

class XDGImplement
{
public:

	static void GetIpInfo(TFunction<void(TSharedPtr<FIpInfoModel> model, FString msg)> resultBlock);

	static void InitSDK(FString sdkClientId, TFunction<void(bool successed, FString msg)> resultBlock);

	
};
