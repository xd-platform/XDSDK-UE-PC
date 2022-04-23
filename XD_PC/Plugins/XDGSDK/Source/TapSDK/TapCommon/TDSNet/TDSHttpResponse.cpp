#include "TDSHttpResponse.h"

#include "JsonHelper.h"
#include "SEditorViewportToolBarMenu.h"
#include "TDSHttpRequest.h"

// TDSHttpResponse::~TDSHttpResponse()
// {
// 	UE_LOG(LogTemp, Display, TEXT("TDSHttpResponse销毁了-----"));
// }

FString TDSHttpResponse::GenerateDebugString()
{
	FString DebugString;
	if (request == nullptr)
	{
		return DebugString;
	}
	DebugString += "------------------------------------------\n";
	DebugString += (request->type == TDSHttpRequest::Post ? "POST: " : "GET: ") + request->GetFinalUrl() + "\n";
	if (request->Headers.Num() > 0)
	{
		DebugString += "Headers:\n";
		for (auto Header : request->Headers)
		{
			DebugString += "\t" + Header.Key + ": " + Header.Value + "\n";
		}
	}
	if (request->type == TDSHttpRequest::Post && request->Parameters->Values.Num() > 0)
	{
		FString body = JsonHelper::GetJsonString(request->Parameters);
		DebugString += "Body:\n";
		DebugString += body + "\n";
	}

	if (state == networkError)
	{
		DebugString += "Network Error, please check network connection\n";
	} else
	{
		DebugString += "HttpCode: " + FString::FromInt(httpCode) + "\n";
		DebugString += "Response:\n" + contentString + "\n";
	}
	DebugString += "------------------------------------------";
	return DebugString;
}
