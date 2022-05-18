#pragma once

#include "TUDBEnum.h"
#include "TUHttpRequest.h"



class TUDBNet: public TUHttpRequest
{
	
public:

	static void SendEvent(TUDBEnum::EventType EventType, TSharedPtr<FJsonObject> Paras);

	// 普通事件
	static void SendNormalEvent(TSharedPtr<FJsonObject> Paras);

	// 自定义事件
	static void SendCustomEvent(TSharedPtr<FJsonObject> Paras);

	// 设备/用户 登录事件
	static void SendIdentifyEvent(TSharedPtr<FJsonObject> Paras);

	static int CacheCount;

private:

	static void SendEvent(const FString& Url, TSharedPtr<FJsonObject> Paras);

	static void PerformRequest(const TSharedPtr<TUDBNet>& Request);

	static TQueue<TSharedPtr<TUDBNet>> RequestQueue;

	static FTimerHandle RetryTimerHandle;


};
