#include "TAUDBNet.h"

#include "TAUDBConfig.h"
#include "TDUDebuger.h"
#include "TDUHttpManager.h"

TQueue<TSharedPtr<TAUDBNet>> TAUDBNet::RequestQueue;
FTimerHandle TAUDBNet::RetryTimerHandle = FTimerHandle();
int TAUDBNet::CacheCount = 0;


void TAUDBNet::SendEvent(TAUDBEnum::EventType EventType, TSharedPtr<FJsonObject> Paras)
{
	if (EventType == TAUDBEnum::Normal)
	{
		SendNormalEvent(Paras);
	} else if (EventType == TAUDBEnum::Custom)
	{
		SendCustomEvent(Paras);
	} else if (EventType == TAUDBEnum::Identify)
	{
		SendIdentifyEvent(Paras);
	}
}

void TAUDBNet::SendNormalEvent(TSharedPtr<FJsonObject> Paras)
{
	FString Url = TAUDBConfig::GetHost() + "event";
	SendEvent(Url, Paras);
}

void TAUDBNet::SendCustomEvent(TSharedPtr<FJsonObject> Paras)
{
	FString Url = TAUDBConfig::GetHost() + "custom";
	SendEvent(Url, Paras);
}

void TAUDBNet::SendIdentifyEvent(TSharedPtr<FJsonObject> Paras)
{
	FString Url = TAUDBConfig::GetHost() + "identify";
	SendEvent(Url, Paras);
}

void TAUDBNet::SendEvent(const FString& Url, TSharedPtr<FJsonObject> Paras)
{
	const TSharedPtr<TAUDBNet> request = MakeShareable(new TAUDBNet);
	request->URL = Url;
	request->Type = Post;
	request->Parameters = Paras;
	// 查下有没有循环引用
	request->onCompleted.BindLambda([=](TSharedPtr<TDUHttpResponse> Response) {
		if (Response->success)
		{
			// 如果请求成功，那么去掉队列的第一个，执行下一个请求
			if (request== *RequestQueue.Peek())
			{
				RequestQueue.Pop();
				CacheCount--;
			}
			PerformRequest(*RequestQueue.Peek());
		} else
		{
			// 如果请求失败，等待2秒无限重试；
			if (request == *RequestQueue.Peek() && GWorld)
			{
				GWorld->GetWorld()->GetTimerManager().SetTimer(RetryTimerHandle, [=]()
				{
					PerformRequest(*RequestQueue.Peek());
				}, 2, false);
			}
		}
	});
	PerformRequest(request);
}

void TAUDBNet::PerformRequest(TSharedPtr<TAUDBNet> Request)
{
	if (!Request.IsValid())
	{
		return;
	}
	// 如果缓存的请求超过500个，那么直接清空队列，执行新的请求。
	if (CacheCount >= 500)
	{
		if (GWorld)
		{
			GWorld->GetWorld()->GetTimerManager().ClearTimer(RetryTimerHandle);
		}
		RequestQueue.Empty();
		CacheCount = 0;
	}
	// 如果需要执行的请求不是队列的第一个，那么先加到队列里
	if (Request != *RequestQueue.Peek())
	{
		RequestQueue.Enqueue(Request);
		CacheCount++;
	}
	// 如果加到队列里之后，队列的第一个请求还不是要执行的请求，说明有其他请求正在执行，等着执行就行
	if (Request != *RequestQueue.Peek())
	{
		return;
	}
	TDUHttpManager::Get().request(Request);
	if (CacheCount >= 1)
	{
		TDUDebuger::WarningLog(FString::Printf(TEXT("TapDB 缓存请求数量：%d"), CacheCount));
	}
}
