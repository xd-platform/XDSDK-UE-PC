#include "TDSHttpManager.h"
#include "Http.h"

TDSHttpManager* TDSHttpManager::Singleton = NULL;

TDSHttpManager& TDSHttpManager::Get()
{
	if (Singleton == NULL)
	{
		check(IsInGameThread());
		Singleton = new TDSHttpManager();
	}
	return *Singleton;
}

TSharedRef<IHttpRequest, ESPMode::ThreadSafe> GenerateRequest(TSharedPtr<TDSHttpRequest> tdsReq)
{
	auto Request = FHttpModule::Get().CreateRequest();
	//This is the url on which to process the request
	
	Request->SetTimeout(tdsReq->timeoutSecs);
	
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
	headers.Append(tdsReq->headers);
	parameters->Values.Append(tdsReq->parameters->Values);
	tdsReq->headers = headers;
	tdsReq->parameters = parameters;
	
	for (auto header : headers)
	{
		Request->SetHeader(header.Key, header.Value);
	}
	
	switch (tdsReq->type)
	{
	case TDSHttpRequest::Type::Get:
		{
			Request->SetVerb("GET");
			FString queryString = TDSHttpManager::CombinParameters(parameters);
			FString url = tdsReq->URL;
			if (queryString.Len() > 0)
			{
				queryString = FGenericPlatformHttp::UrlEncode(queryString);
				url = url + "?" + queryString;
			}
			Request->SetURL(url);
		}
		break;
	case TDSHttpRequest::Type::Post:
		{
			Request->SetVerb("POST");
			Request->SetURL(tdsReq->URL);
			FString body;
			TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&body);
			FJsonSerializer::Serialize(parameters.ToSharedRef(), Writer);
			// 目前只支持了json格式的body，如果需要其他的，还要继续封装。
			Request->SetContentAsString(body);
		}
		break;
	}
	return Request;
}

// FString combine
FString TDSHttpManager::CombinParameters(TSharedPtr<FJsonObject> parameters)
{
	TArray<FString> keyValues;
	for (auto para : parameters->Values)
	{
		FString value;
		if (para.Value->TryGetString(value))
		{
			keyValues.Add(para.Key + "=" + value);
		}
	}
	return FString::Join(keyValues, TEXT("&"));
}

void TDSHttpManager::request(TSharedPtr<TDSHttpRequest> tdsReq)
{
	auto Request = GenerateRequest(tdsReq);
	// Request->OnProcessRequestComplete().BindUObject(this, &TDSHttpRequest::OnResponseReceived);
	Request->OnProcessRequestComplete().BindLambda(
		[=](FHttpRequestPtr HttpRequest, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			tdsReq->tryCount++;
			if (bWasSuccessful == false || !EHttpResponseCodes::IsOk(Response->GetResponseCode()))
			{
				if (tdsReq->tryCount < tdsReq->repeatCount)
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
			UE_LOG(TDSHttpLog, Warning, TEXT("%s"), *tdsRes->GenerateDebugString());
			tdsReq->onCompleted.ExecuteIfBound(tdsRes);
		});
	Request->ProcessRequest();
}
