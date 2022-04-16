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

void TDSHttpManager::request(TSharedPtr<TDSHttpRequest> tdsReq)
{
	auto Request = FHttpModule::Get().CreateRequest();
	//This is the url on which to process the request
	Request->SetURL(tdsReq->URL);
	switch (tdsReq->type)
	{
	case TDSHttpRequest::Type::Get:
		Request->SetVerb("GET");
		break;
	case TDSHttpRequest::Type::Post:
		Request->SetVerb("POST");
		break;
	}
	for (auto header : tdsReq->headers)
	{
		Request->SetHeader(header.Key, header.Value);
	}

	// Request->OnProcessRequestComplete().BindUObject(this, &TDSHttpRequest::OnResponseReceived);
	Request->OnProcessRequestComplete().BindLambda(
		[=](FHttpRequestPtr HttpRequest, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			tdsReq->tryCount++;
			if (bWasSuccessful == false || !EHttpResponseCodes::IsOk(Response->GetResponseCode()))
			{
				if (tdsReq->tryCount < tdsReq->repeatCount) { request(tdsReq); }
			}
			TSharedPtr<TDSHttpResponse> tdsRes(new TDSHttpResponse);
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
				UE_LOG(TDSHttpLog, Display, TEXT("%s\nHttpCode: %d\nResponse:\n%s"), * tdsReq->URL, tdsRes->httpCode, *Response->GetContentAsString());
			} else
			{
				tdsRes->state = TDSHttpResponse::networkError;
				UE_LOG(TDSHttpLog, Warning, TEXT("%s\nNetwork Error, please check network connection"), *Response->GetURL());
			}
			tdsReq->onCompleted.ExecuteIfBound(tdsRes);
		});
	Request->ProcessRequest();
}
