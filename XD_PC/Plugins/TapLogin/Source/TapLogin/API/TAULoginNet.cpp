#include "TAULoginNet.h"

#include "DeviceInfo.h"
#include "JsonHelper.h"
#include "TapTapSdk.h"
#include "TDSCrypto.h"
#include "TDSHelper.h"
#include "TDUHttpManager.h"
#include "URLParser.h"

TAULoginNet::TAULoginNet()
{
}

FTAULoginError TAULoginNet::GenerateErrorInfo(const TSharedPtr<TDUHttpResponse>& Response)
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
	FTAULoginError Error = TAULoginNet::GenerateErrorInfo(Response);

	auto JsonObject = JsonHelper::GetJsonObject(Response->contentString);
	bool Success = false;
	const TSharedPtr<FJsonObject>* DataJsonObject = nullptr;
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

void TAULoginNet::RequestAccessToken(const FString& DeviceCode, TFunction<void(TSharedPtr<FTapAccessToken> Model, FTAULoginError Error)> callback)
{
	const TSharedPtr<TDUHttpRequest> request = MakeShareable(new TAULoginNet());
	request->Type = Post;
	request->URL = TapTapSdk::CurrentRegion->TokenUrl();
	request->Parameters->SetStringField("grant_type", "device_token");
	request->Parameters->SetStringField("client_id", TapTapSdk::ClientId);
	request->Parameters->SetStringField("secret_type", "hmac-sha-1");
	request->Parameters->SetStringField("code", DeviceCode);
	request->Parameters->SetStringField("version", "1.0");
	request->Parameters->SetStringField("platform", "ue");
	request->Parameters->SetStringField("info", FString::Printf(TEXT("{\"device_id\":\"%s\"}"), *DeviceInfo::GetLoginId()));

	request->onCompleted.BindLambda([=](TSharedPtr<TDUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, callback);
	});
	TDUHttpManager::Get().request(request);
}

void TAULoginNet::RequestProfile(const FTapAccessToken& AccessToken,
	TFunction<void(TSharedPtr<FTAUProfileModel> Model, FTAULoginError Error)> callback)
{
	const TSharedPtr<TAULoginNet> request = MakeShareable(new TAULoginNet());
	request->URL = TapTapSdk::CurrentRegion->ProfileUrl();
	request->Parameters->SetStringField("client_id", TapTapSdk::ClientId);
	request->AccessToken = MakeShareable(new FTapAccessToken(AccessToken));
	request->onCompleted.BindLambda([=](TSharedPtr<TDUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, callback);
	});
	TDUHttpManager::Get().request(request);
}

void TAULoginNet::RequestAccessTokenFromWeb(const TSharedPtr<FJsonObject>& Paras,
	TFunction<void(TSharedPtr<FTapAccessToken> Model, FTAULoginError Error)> callback) {
	const TSharedPtr<TAULoginNet> request = MakeShareable(new TAULoginNet());
	request->Type = Post;
	request->URL = TapTapSdk::CurrentRegion->TokenUrl();
	request->Parameters = Paras;
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
	if (AccessToken.IsValid())
	{
		this->Headers.Add("Authorization", GetMacToken());
		return true;
	} else
	{
		return TDUHttpRequest::ResetHeadersBeforeRequest();
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
	auto Parse = TauCommon::FURL_RFC3986();
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
	auto sha1 = TDSCrypto::HmacSHA1(TDSCrypto::UTF8Encode(dataStr), TDSCrypto::UTF8Encode(tokenModel->mac_key));
	FString mac = TDSCrypto::Base64Encode(sha1);
	
	authToken = FString::Printf(TEXT("MAC id=\"%s\",ts=\"%s\",nonce=\"%s\",mac=\"%s\""), *tokenModel->kid, *timeStr, *nonce, *mac);
	return authToken;
}


