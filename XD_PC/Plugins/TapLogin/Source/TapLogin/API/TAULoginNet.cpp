#include "TAULoginNet.h"

#include "TUDeviceInfo.h"
#include "TUJsonHelper.h"
#include "TapTapSdk.h"
#include "TUCrypto.h"
#include "TDSHelper.h"
#include "TUHttpManager.h"
#include "URLParser.h"

TAULoginNet::TAULoginNet()
{
}

FTAULoginError TAULoginNet::GenerateErrorInfo(const TSharedPtr<TUHttpResponse>& Response)
{
	FTAULoginError Error = FTAULoginError();
	if (Response->state == TUHttpResponse::clientError)
	{
		Error.code = TUHttpResponse::clientError;
		Error.msg = "request fail";
	} else if (Response->state == TUHttpResponse::serverError)
	{
		Error.code = TUHttpResponse::serverError;
		Error.msg = "server error";
	} else if (Response->state == TUHttpResponse::networkError)
	{
		Error.code = TUHttpResponse::networkError;
		Error.msg = "network connection error";
	}
	return Error;
}

template <typename StructType>
void PerfromWrapperResponseCallBack(const TSharedPtr<TUHttpResponse>& Response, TFunction<void(TSharedPtr<StructType> Model, FTAULoginError Error)> Callback)
{
	if (Callback == nullptr)
	{
		return;
	}
	FTAULoginError Error = TAULoginNet::GenerateErrorInfo(Response);

	auto JsonObject = TUJsonHelper::GetJsonObject(Response->contentString);
	bool Success = false;
	const TSharedPtr<FJsonObject>* DataJsonObject = nullptr;
	if (JsonObject.IsValid() && JsonObject->TryGetBoolField("success", Success) && JsonObject->TryGetObjectField("data", DataJsonObject))
	{
		if (Success)
		{
			auto Model = TUJsonHelper::GetUStruct<StructType>(*DataJsonObject);
			if (Model.IsValid())
			{
				Callback(Model, Error);
				return;
			}
		} else
		{
			auto Model = TUJsonHelper::GetUStruct<FTAULoginError>(*DataJsonObject);
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
	const TSharedPtr<TUHttpRequest> request = MakeShareable(new TAULoginNet());
	request->Type = Post;
	request->URL = TapTapSdk::CurrentRegion->CodeUrl();
	request->Parameters->SetStringField("client_id", TapTapSdk::ClientId);
	request->Parameters->SetStringField("response_type", "device_code");
	request->Parameters->SetStringField("scope", FString::Join(Permissions, TEXT(",")));
	request->Parameters->SetStringField("version", TapTapSdk::Version);
	request->Parameters->SetStringField("platform", "ue");
	request->Parameters->SetStringField("info", FString::Printf(TEXT("{\"device_id\":\"%s\"}"), *TUDeviceInfo::GetLoginId()));

	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, callback);
	});
	TUHttpManager::Get().request(request);
}

void TAULoginNet::RequestAccessToken(const FString& DeviceCode, TFunction<void(TSharedPtr<FTapAccessToken> Model, FTAULoginError Error)> callback)
{
	const TSharedPtr<TUHttpRequest> request = MakeShareable(new TAULoginNet());
	request->Type = Post;
	request->URL = TapTapSdk::CurrentRegion->TokenUrl();
	request->Parameters->SetStringField("grant_type", "device_token");
	request->Parameters->SetStringField("client_id", TapTapSdk::ClientId);
	request->Parameters->SetStringField("secret_type", "hmac-sha-1");
	request->Parameters->SetStringField("code", DeviceCode);
	request->Parameters->SetStringField("version", "1.0");
	request->Parameters->SetStringField("platform", "ue");
	request->Parameters->SetStringField("info", FString::Printf(TEXT("{\"device_id\":\"%s\"}"), *TUDeviceInfo::GetLoginId()));

	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, callback);
	});
	TUHttpManager::Get().request(request);
}

void TAULoginNet::RequestProfile(const FTapAccessToken& AccessToken,
	TFunction<void(TSharedPtr<FTAUProfileModel> Model, FTAULoginError Error)> callback)
{
	const TSharedPtr<TAULoginNet> request = MakeShareable(new TAULoginNet());
	request->URL = TapTapSdk::CurrentRegion->ProfileUrl();
	request->Parameters->SetStringField("client_id", TapTapSdk::ClientId);
	request->AccessToken = MakeShareable(new FTapAccessToken(AccessToken));
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, callback);
	});
	TUHttpManager::Get().request(request);
}

void TAULoginNet::RequestAccessTokenFromWeb(const TSharedPtr<FJsonObject>& Paras,
	TFunction<void(TSharedPtr<FTapAccessToken> Model, FTAULoginError Error)> callback) {
	const TSharedPtr<TAULoginNet> request = MakeShareable(new TAULoginNet());
	request->Type = Post;
	request->URL = TapTapSdk::CurrentRegion->TokenUrl();
	request->Parameters = Paras;
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, callback);
	});
	TUHttpManager::Get().request(request);
}

TMap<FString, FString> TAULoginNet::CommonHeaders()
{
	return TUHttpRequest::CommonHeaders();
}

TSharedPtr<FJsonObject> TAULoginNet::CommonParameters()
{
	return TUHttpRequest::CommonParameters();
}

bool TAULoginNet::ResetHeadersBeforeRequest()
{
	if (AccessToken.IsValid())
	{
		this->Headers.Add("Authorization", GetMacToken());
		return true;
	} else
	{
		return TUHttpRequest::ResetHeadersBeforeRequest();
	}
	
}

FString TAULoginNet::GetMacToken()
{
	auto tokenModel = AccessToken;
	FString authToken;
	if (tokenModel == nullptr)
	{
		return authToken;
	}
	auto Parse = TUCommon::FURL_RFC3986();
	Parse.Parse(this->GetFinalUrl());
	FString timeStr = FString::Printf(TEXT("%lld"), FDateTime::UtcNow().ToUnixTimestamp());
	FString nonce = TDSHelper::GetRandomStr(5);
	FString md = this->Type == Get ? "GET" : "POST";

	FString pathAndQuery = Parse.GetPath();
	if (Parse.GetQuery().Len() > 0)
	{
		pathAndQuery += "?" + Parse.GetQuery();
	}
	FString domain = Parse.GetHost().ToLower();
	FString port = Parse.GetPort();

	FString dataStr = timeStr + "\n" + nonce + "\n" + md + "\n" + pathAndQuery + "\n" + domain + "\n" + port + "\n\n";
	auto sha1 = TUCrypto::HmacSHA1(TUCrypto::UTF8Encode(dataStr), TUCrypto::UTF8Encode(tokenModel->mac_key));
	FString mac = TUCrypto::Base64Encode(sha1);
	
	authToken = FString::Printf(TEXT("MAC id=\"%s\",ts=\"%s\",nonce=\"%s\",mac=\"%s\""), *tokenModel->kid, *timeStr, *nonce, *mac);
	return authToken;
}


