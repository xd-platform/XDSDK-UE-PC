#pragma once
#include "TapError.h"
#include "TDUHttpRequest.h"

class TAUDBNet: public TDUHttpRequest
{
	
public:

	// 普通事件
	static void SendNormalEvent(TSharedPtr<FJsonObject> Paras);

	// 自定义事件
	static void SendCustomEvent(TSharedPtr<FJsonObject> Paras);

	// 设备/用户 登录事件
	static void SendIdentifyEvent(TSharedPtr<FJsonObject> Paras);

private:

	static void SendEvent(const FString& Url, TSharedPtr<FJsonObject> Paras);

	static void PerformRequest(TSharedPtr<TAUDBNet> Request);

	static TQueue<TSharedPtr<TAUDBNet>> RequestQueue;

	static FTimerHandle RetryTimerHandle;

	static int CacheCount;
};
