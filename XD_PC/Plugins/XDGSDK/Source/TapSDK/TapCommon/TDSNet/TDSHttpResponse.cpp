#include "TDSHttpResponse.h"

#include <string>

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
	DebugString += "URL: " + request->URL + "\n";
	if (request->headers.Num() > 0)
	{
		DebugString += "Headers:\n";
		for (auto Header : request->headers)
		{
			DebugString += "\t" + Header.Key + ": " + Header.Value + "\n";
		}
	}
	if (request->parameters->Values.Num() > 0)
	{
		FString body = JsonHelper::GetJsonString(request->parameters);
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
