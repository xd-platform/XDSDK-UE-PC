#include "TDSHttpRequest.h"
#include "Http.h"


TDSHttpRequest::TDSHttpRequest()
{
	Parameters = MakeShareable(new FJsonObject);
	PostUrlParameters = MakeShareable(new FJsonObject);
}

FString TDSHttpRequest::GetFinalUrl()
{
	return FinalURL;
}

// TDSHttpRequest::~TDSHttpRequest()
// {
// 	UE_LOG(LogTemp, Display, TEXT("TDSHttpRequest销毁了----"));
// }




