#include "TDUHttpRequest.h"
#include "Http.h"


TDUHttpRequest::TDUHttpRequest()
{
	Parameters = MakeShareable(new FJsonObject);
	PostUrlParameters = MakeShareable(new FJsonObject);
}

FString TDUHttpRequest::GetFinalUrl()
{
	return FinalURL;
}

TMap<FString, FString> TDUHttpRequest::CommonHeaders()
{
	TMap<FString, FString> CommonHeaders;
	if (Form == Default)
	{
		CommonHeaders.Add("Content-Type", "application/x-www-form-urlencoded");
	} else if (Form == Json)
	{
		CommonHeaders.Add("Content-Type", "application/json");
	}
	return CommonHeaders;
}

TSharedPtr<FJsonObject> TDUHttpRequest::CommonParameters()
{
	return MakeShareable(new FJsonObject);
}

// TDUHttpRequest::~TDUHttpRequest()
// {
// 	UE_LOG(LogTemp, Display, TEXT("TDUHttpRequest销毁了----"));
// }




