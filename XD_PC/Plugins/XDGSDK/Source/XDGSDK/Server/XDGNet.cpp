#include "XDGNet.h"

#include <string>

#include "JsonObjectConverter.h"
#include "LanguageManager.h"
#include "DataStorageName.h"
#include "IpInfoModel.h"
#include "DeviceInfo.h"
#include "JsonHelper.h"
#include "TDSCrypto.h"
#include "TokenModel.h"
#include "UrlParse.h"
#include "XDGSDK.h"

// public readonly static string BASE_URL = "https://test-xdsdk-intnl-6.xd.com"; //测试
static FString BASE_URL = "https://xdsdk-intnl-6.xd.com"; //正式

//获取配置
static FString INIT_SDK_URL = BASE_URL + "/api/init/v1/config";

//IP信息
static FString IP_INFO = "https://ip.xindong.com/myloc2";

// login
static FString XDG_USER_PROFILE = BASE_URL + "/api/account/v1/info";

//游客
static FString XDG_COMMON_LOGIN = BASE_URL + "/api/login/v1/union";

// 查询补款订单信息
static FString XDG_PAYBACK_LIST = BASE_URL + "/order/v1/user/repayOrders";

FString GetRandomStr(int length){
	FString LetterStr = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	FString SB;
	for (int i = 0; i < length; i++){
		SB += LetterStr[FMath::RandRange(0, LetterStr.Len()-1)];
	}

	return SB;
}

XDGNet::XDGNet()
{
	
}


TMap<FString, FString> XDGNet::CommonHeaders()
{
	TMap<FString, FString> headers;
	headers.Add("Content-Type", "application/json;charset=utf-8");
	headers.Add("Accept-Language", LanguageManager::GetLanguageKey());
	return headers;
}

TSharedPtr<FJsonObject> XDGNet::CommonParameters()
{
	TSharedPtr<FJsonObject> query = MakeShareable(new FJsonObject);
	
	query->SetStringField("clientId", DataStorage::LoadString(DataStorageName::ClientId));

	query->SetStringField("sdkLang", LanguageManager::GetLanguageKey());
	query->SetStringField("lang", LanguageManager::GetLanguageKey());

	auto ipInfoModel = DataStorage::LoadStruct<FIpInfoModel>(DataStorageName::IpInfo);
	if (ipInfoModel != nullptr)
	{
		ipInfoModel = MakeShareable(new FIpInfoModel);
	}
	query->SetStringField("loc", ipInfoModel->country_code);
	query->SetStringField("city", ipInfoModel->city);
	query->SetStringField("timeZone", ipInfoModel->timeZone);
	query->SetStringField("countryCode", ipInfoModel->country_code);
	
	query->SetStringField("locationInfoType", "ip");
	query->SetStringField("chn", "PC");

	query->SetStringField("sdkVer", FXDGSDKModule::VersionName);

	query->SetStringField("did", DeviceInfo::GetLoginId());
	query->SetStringField("pt", DeviceInfo::GetPlatform());
	query->SetStringField("pkgName", DeviceInfo::GetProjectName());
	query->SetStringField("os", DeviceInfo::GetOSVersion());
	query->SetStringField("res", FString::Printf(TEXT("%d_%d"), DeviceInfo::GetScreenWidth(), DeviceInfo::GetScreenHeight()));
	
	query->SetStringField("time", FString::Printf(TEXT("%lld"), FDateTime::UtcNow().ToUnixTimestamp()));
	
	
	// 		   {"appId", cfgMd == null ? "" : cfgMd.data.configs.appId + ""},
	// 		   {"mem", SystemInfo.systemMemorySize / 1024 + "GB"},
	// 		   {"mod", SystemInfo.deviceModel},
	// 		   {"brand", SystemInfo.graphicsDeviceVendor},
	// 		   {"appVer", Application.version},
	// 		   {"appVerCode", Application.version},
	return query;
}

void XDGNet::DoSomeingAfterCombinHeadersAndParas()
{
	auto auth = GetMacToken();
	if (auth.Len() > 0)
	{
		this->headers.Add("Authorization", auth);
	}
}



FXDGError GenerateErrorInfo(TSharedPtr<TDSHttpResponse>& response)
{
	FXDGError error;
	if (response->state == TDSHttpResponse::clientError)
	{
		error.code = TDSHttpResponse::clientError;
		error.msg = "request fail";
	} else if (response->state == TDSHttpResponse::serverError)
	{
		error.code = TDSHttpResponse::serverError;
		error.msg = "server error";
	} else if (response->state == TDSHttpResponse::networkError)
	{
		error.code = TDSHttpResponse::networkError;
		error.msg = "network connection error";
	}
	return error;
}

template <typename StructType>
TSharedPtr<StructType> GenerateStructPtr(TSharedPtr<TDSHttpResponse>& response)
{
	if (response != nullptr && response->state == TDSHttpResponse::success) {
		return JsonHelper::GetUStruct<StructType>(response->contentString);
	} else {
		return nullptr;
	}
}

template <typename StructType>
void PerfromCallBack(TSharedPtr<TDSHttpResponse>& response, TFunction<void(TSharedPtr<StructType> model, FXDGError error)> callback)
{
	if (callback == nullptr)
	{
		return;
	}
	TSharedPtr<StructType> model = GenerateStructPtr<StructType>(response);
	FXDGError error = GenerateErrorInfo(response);
	if (model == nullptr && error.code == 0)
	{
		error.code = TDSHttpResponse::clientError;
		error.msg = "json parse error";
	}
	callback(model, error);
}

void XDGNet::RequestIpInfo(TFunction<void(TSharedPtr<FIpInfoModel> model, FXDGError error)> callback)
{
	const TSharedPtr<TDSHttpRequest> request = MakeShareable(new XDGNet());
	request->URL = IP_INFO;
	request->isPure = true;
	request->repeatCount = 3;
	request->onCompleted.BindLambda([=](TSharedPtr<TDSHttpResponse> response) {
		PerfromCallBack(response, callback);
	});
	TDSHttpManager::Get().request(request);
}

void XDGNet::RequestConfig(TFunction<void(TSharedPtr<FInitConfigModel> model, FXDGError error)> callback)
{
	const TSharedPtr<TDSHttpRequest> request = MakeShareable(new XDGNet());
	request->URL = INIT_SDK_URL;
	request->onCompleted.BindLambda([=](TSharedPtr<TDSHttpResponse> response) {
		// PerfromCallBack(response, callback);
	});
	TDSHttpManager::Get().request(request);
}




FString XDGNet::GetMacToken() {
	auto tokenModel = FTokenModel::GetCurrentToken();
	FString authToken;
	if (tokenModel == nullptr)
	{
		return authToken;
	}
	UrlParse parse(this->URL);
	FString timeStr = FString::Printf(TEXT("%lld"), FDateTime::UtcNow().ToUnixTimestamp());
	FString nonce = GetRandomStr(5);
	FString md = this->type == Get ? "GET" : "POST";

	FString pathAndQuery = "/" + parse.Path;
	if (parse.query.Len() > 0)
	{
		pathAndQuery += "?" + parse.query;
	}
	FString domain = parse.Host.ToLower();
	FString port = parse.Port;

	FString dataStr = timeStr + "\n" + nonce + "\n" + md + "\n" + pathAndQuery + "\n" + domain + "\n" + port + "\n";
	auto sha1 = TDSCrypto::HmacSHA1(TDSCrypto::UTF8Encode(dataStr), TDSCrypto::UTF8Encode(tokenModel->macKey));
	FString mac = TDSCrypto::Base64Encode(sha1);
	
	authToken = FString::Printf(TEXT("MAC id=\"%s\",ts=\"%s\",nonce=\"%s\",mac=\"%s\""), *tokenModel->kid, *timeStr, *nonce, *mac);
	return authToken;
}





// private static string GetMacToken(string url, string method){
// 	TokenModel model = TokenModel.GetLocalModel();
// 	string authToken = null;
// 	if (model != null && model.data != null){
// 		var uri = new Uri(url);
// 		var timeStr = new DateTimeOffset(DateTime.UtcNow).ToUnixTimeSeconds() + "";
// 		var nonce = GetRandomStr(5);
// 		var md = method.ToUpper();
//
// 		var pathAndQuery = uri.PathAndQuery;
// 		var domain = uri.Host.ToLower();
// 		var port = uri.Port + "";
//
// 		var dataStr = $"{timeStr}\n{nonce}\n{md}\n{pathAndQuery}\n{domain}\n{port}\n";
// 		var mac = Base64WithSecret(model.data.macKey, dataStr);
// 		authToken = $"MAC id=\"{model.data.kid}\",ts=\"{timeStr}\",nonce=\"{nonce}\",mac=\"{mac}\"";
// 	}
//
// 	return authToken;
// }
