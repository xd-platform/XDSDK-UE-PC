#include "TDUHttpManager.h"

#include "Http.h"
#include "JsonHelper.h"
#include "TDSHelper.h"

TDUHttpManager* TDUHttpManager::Singleton = nullptr;

TDUHttpManager& TDUHttpManager::Get()
{
	if (Singleton == nullptr)
	{
		check(IsInGameThread());
		Singleton = new TDUHttpManager();
	}
	return *Singleton;
}

TSharedRef<IHttpRequest, ESPMode::ThreadSafe> GenerateRequest(TSharedPtr<TDUHttpRequest> tdsReq)
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
	
	switch (tdsReq->Type)
	{
	case TDUHttpRequest::Type::Get:
		{
			Request->SetVerb("GET");
			FString queryString = TDSHelper::CombinParameters(tdsReq->Parameters);
			FString url = tdsReq->URL;
			if (queryString.Len() > 0)
			{
				url = url + "?" + queryString;
			}
			Request->SetURL(url);
		}
		break;
	case TDUHttpRequest::Type::Post:
		{
			Request->SetVerb("POST");
			FString queryString = TDSHelper::CombinParameters(tdsReq->PostUrlParameters);
			FString url = tdsReq->URL;
			if (queryString.Len() > 0)
			{
				url = url + "?" + queryString;
			}
			Request->SetURL(url);
			if (tdsReq->Form == TDUHttpRequest::Form::Default)
			{
				FString body = TDSHelper::CombinParameters(tdsReq->Parameters);
				Request->SetContentAsString(body);
			} else if (tdsReq->Form == TDUHttpRequest::Form::Json)
			{
				FString body = JsonHelper::GetJsonString(tdsReq->Parameters);
				Request->SetContentAsString(body);
			}
			
		}
		break;
	}
	return Request;
}

void TDUHttpManager::request(TSharedPtr<TDUHttpRequest> tdsReq)
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
			TSharedPtr<TDUHttpResponse> tdsRes(new TDUHttpResponse);
			tdsRes->request = tdsReq;
			if (bWasSuccessful)
			{
				tdsRes->httpCode = Response->GetResponseCode();
				tdsRes->contentString = Response->GetContentAsString();
				tdsRes->headers = Response->GetAllHeaders();
				if (EHttpResponseCodes::IsOk(tdsRes->httpCode))
				{
					tdsRes->state = TDUHttpResponse::success;
				} else if (tdsRes->httpCode >= 500)
				{
					tdsRes->state = TDUHttpResponse::serverError;
				} else
				{
					tdsRes->state = TDUHttpResponse::clientError;
				}
			} else
			{
				tdsRes->state = TDUHttpResponse::networkError;
			}
			UE_LOG(TDUHttpLog, Display, TEXT("%s"), *tdsRes->GenerateDebugString());
			AsyncTask(ENamedThreads::GameThread, [=]()
			{
				tdsReq->onCompleted.ExecuteIfBound(tdsRes);
			});
		});
	Request->ProcessRequest();
}
