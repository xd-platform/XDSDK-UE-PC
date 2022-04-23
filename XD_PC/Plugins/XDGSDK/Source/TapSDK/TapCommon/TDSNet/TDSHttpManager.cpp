#include "TDSHttpManager.h"
#include "Http.h"
#include "JsonHelper.h"

TDSHttpManager* TDSHttpManager::Singleton = nullptr;

TDSHttpManager& TDSHttpManager::Get()
{
	if (Singleton == nullptr)
	{
		check(IsInGameThread());
		Singleton = new TDSHttpManager();
	}
	return *Singleton;
}

TSharedRef<IHttpRequest, ESPMode::ThreadSafe> GenerateRequest(TSharedPtr<TDSHttpRequest> tdsReq)
{
	//This is the url on which to process the request
	
	TMap<FString, FString> headers;
	TSharedPtr<FJsonObject> parameters = MakeShareable(new FJsonObject);
	if (!tdsReq->isPure)
	{
		headers.Append(tdsReq->CommonHeaders());
		TSharedPtr<FJsonObject> CommonParameters = tdsReq->CommonParameters();
		if (CommonParameters != nullptr)
		{
			parameters->Values.Append(CommonParameters->Values);
		}
	}
	headers.Append(tdsReq->Headers);
	parameters->Values.Append(tdsReq->Parameters->Values);
	tdsReq->Headers = headers;
	tdsReq->Parameters = parameters;
	
	auto Request = FHttpModule::Get().CreateRequest();
	Request->SetTimeout(tdsReq->TimeoutSecs);
	for (auto header : tdsReq->Headers)
	{
		Request->SetHeader(header.Key, header.Value);
	}
	
	switch (tdsReq->type)
	{
	case TDSHttpRequest::Type::Get:
		{
			Request->SetVerb("GET");
			FString queryString = TDSHttpManager::CombinParameters(tdsReq->Parameters);
			FString url = tdsReq->URL;
			if (queryString.Len() > 0)
			{
				url = url + "?" + queryString;
			}
			Request->SetURL(url);
		}
		break;
	case TDSHttpRequest::Type::Post:
		{
			Request->SetVerb("POST");
			FString queryString = TDSHttpManager::CombinParameters(tdsReq->PostUrlParameters);
			FString url = tdsReq->URL;
			if (queryString.Len() > 0)
			{
				url = url + "?" + queryString;
			}
			Request->SetURL(url);
			FString body = JsonHelper::GetJsonString(tdsReq->Parameters);
			// 目前只支持了json格式的body，如果需要其他的，还要继续封装。
			Request->SetContentAsString(body);
		}
		break;
	}
	return Request;
}

// FString combine
FString TDSHttpManager::CombinParameters(TSharedPtr<FJsonObject> parameters, bool isEncode)
{
	TArray<FString> keyValues;
	for (auto para : parameters->Values)
	{
		FString value;
		if (para.Value->TryGetString(value))
		{
			keyValues.Add(para.Key + "=" +  (isEncode ? FGenericPlatformHttp::UrlEncode(value) : value));
		}
	}
	return FString::Join(keyValues, TEXT("&"));
}

void TDSHttpManager::request(TSharedPtr<TDSHttpRequest> tdsReq)
{
	auto Request = GenerateRequest(tdsReq);
	tdsReq->FinalURL = Request->GetURL();
	if (tdsReq->ResetHeadersBeforeRequest())
	{
		for (auto header : tdsReq->Headers)
		{
			Request->SetHeader(header.Key, header.Value);
		}
	}
	Request->OnProcessRequestComplete().BindLambda(
		[=](FHttpRequestPtr HttpRequest, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			tdsReq->tryCount++;
			if (bWasSuccessful == false || !EHttpResponseCodes::IsOk(Response->GetResponseCode()))
			{
				if (tdsReq->tryCount < tdsReq->RepeatCount)
				{
					request(tdsReq);
					return;
				}
			}
			TSharedPtr<TDSHttpResponse> tdsRes(new TDSHttpResponse);
			tdsRes->request = tdsReq;
			if (bWasSuccessful)
			{
				tdsRes->httpCode = Response->GetResponseCode();
				tdsRes->contentString = Response->GetContentAsString();
				tdsRes->headers = Response->GetAllHeaders();
				if (EHttpResponseCodes::IsOk(tdsRes->httpCode))
				{
					tdsRes->state = TDSHttpResponse::success;
				} else if (tdsRes->httpCode >= 500)
				{
					tdsRes->state = TDSHttpResponse::serverError;
				} else
				{
					tdsRes->state = TDSHttpResponse::clientError;
				}
			} else
			{
				tdsRes->state = TDSHttpResponse::networkError;
			}
			UE_LOG(TDSHttpLog, Display, TEXT("%s"), *tdsRes->GenerateDebugString());
			tdsReq->onCompleted.ExecuteIfBound(tdsRes);
		});
	Request->ProcessRequest();
}
