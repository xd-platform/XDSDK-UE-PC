#include "TDSHttpRequest.h"
#include "Http.h"


TDSHttpRequest::TDSHttpRequest()
{
	parameters = MakeShareable(new FJsonObject);
}

// TDSHttpRequest::~TDSHttpRequest()
// {
// 	UE_LOG(LogTemp, Display, TEXT("TDSHttpRequest销毁了----"));
// }




