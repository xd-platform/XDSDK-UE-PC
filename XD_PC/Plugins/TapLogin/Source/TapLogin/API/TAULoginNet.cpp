#include "TAULoginNet.h"

#include "DeviceInfo.h"
#include "JsonHelper.h"
#include "TapTapSdk.h"
#include "TDUHttpManager.h"

TAULoginNet::TAULoginNet()
{
}

FTAULoginError GenerateErrorInfo(const TSharedPtr<TDUHttpResponse>& Response)
{
	FTAULoginError Error = FTAULoginError();
	if (Response->state == TDUHttpResponse::clientError)
	{
		Error.code = TDUHttpResponse::clientError;
		Error.msg = "request fail";
	} else if (Response->state == TDUHttpResponse::serverError)
	{
		Error.code = TDUHttpResponse::serverError;
		Error.msg = "server error";
	} else if (Response->state == TDUHttpResponse::networkError)
	{
		Error.code = TDUHttpResponse::networkError;
		Error.msg = "network connection error";
	}
	return Error;
}

template <typename StructType>
void PerfromWrapperResponseCallBack(const TSharedPtr<TDUHttpResponse>& Response, TFunction<void(TSharedPtr<StructType> Model, FTAULoginError Error)> Callback)
{
	if (Callback == nullptr)
	{
		return;
	}
	FTAULoginError Error = GenerateErrorInfo(Response);

	auto JsonObject = JsonHelper::GetJsonObject(Response->contentString);
	bool Success = false;
	const TSharedPtr<FJsonObject>* DataJsonObject;
	if (JsonObject.IsValid() && JsonObject->TryGetBoolField("success", Success) && JsonObject->TryGetObjectField("data", DataJsonObject))
	{
		if (Success)
		{
			auto Model = JsonHelper::GetUStruct<StructType>(*DataJsonObject);
			if (Model.IsValid())
			{
				Callback(Model, Error);
				return;
			}
		} else
		{
			auto Model = JsonHelper::GetUStruct<FTAULoginError>(*DataJsonObject);
			if (Model.IsValid())
			{
				Error = *Model.Get();
			}
		}
	}

	Callback(nullptr, Error);
}

void TAULoginNet::RequestLoginQrCode(const TArray<FString> Permissions,
	TFunction<void(TSharedPtr<FTAUQrCodeModel> Model, FTAULoginError Error)> callback)
{
	const TSharedPtr<TDUHttpRequest> request = MakeShareable(new TAULoginNet());
	request->Type = Post;
	request->URL = TapTapSdk::CurrentRegion->CodeUrl();
	request->Parameters->SetStringField("client_id", TapTapSdk::ClientId);
	request->Parameters->SetStringField("response_type", "device_code");
	request->Parameters->SetStringField("scope", FString::Join(Permissions, TEXT(",")));
	request->Parameters->SetStringField("version", TapTapSdk::Version);
	request->Parameters->SetStringField("platform", "ue");
	request->Parameters->SetStringField("info", FString::Printf(TEXT("{\"device_id\":\"%s\"}"), *DeviceInfo::GetLoginId()));

	request->onCompleted.BindLambda([=](TSharedPtr<TDUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, callback);
	});
	TDUHttpManager::Get().request(request);
}

TMap<FString, FString> TAULoginNet::CommonHeaders()
{
	return TDUHttpRequest::CommonHeaders();
}

TSharedPtr<FJsonObject> TAULoginNet::CommonParameters()
{
	return TDUHttpRequest::CommonParameters();
}

bool TAULoginNet::ResetHeadersBeforeRequest()
{
	return TDUHttpRequest::ResetHeadersBeforeRequest();
}

// FString TAULoginNet::GetMacToken()
// {
// }
