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

// TDUHttpRequest::~TDUHttpRequest()
// {
// 	UE_LOG(LogTemp, Display, TEXT("TDUHttpRequest销毁了----"));
// }




