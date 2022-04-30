#include "XDGNet.h"
#include "JsonObjectConverter.h"
#include "LanguageManager.h"
#include "XDGDataStorageName.h"
#include "IpInfoModel.h"
#include "DeviceInfo.h"
#include "JsonHelper.h"
#include "TDSCrypto.h"
#include "TDSHelper.h"
#include "TokenModel.h"
#include "UrlParse.h"
#include "XDGResponseModel.h"
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

// 与leanClound同步
static FString XDG_LOGIN_SYN = BASE_URL + "/api/login/v1/syn";

// 获取用户绑定信息
static FString XDG_BIND_LIST = BASE_URL + "/api/account/v1/bind/list";

// 绑定接口
static FString XDG_BIND_INTERFACE = BASE_URL + "/api/account/v1/bind";

// 解绑接口
static FString XDG_UNBIND_INTERFACE = BASE_URL + "/api/account/v1/unbind";

// 查询补款订单信息
static FString XDG_PAYBACK_LIST = BASE_URL + "/order/v1/user/repayOrders";

XDGNet::XDGNet()
{
	Form = Json;
}


TMap<FString, FString> XDGNet::CommonHeaders()
{
	TMap<FString, FString> headers = TDUHttpRequest::CommonHeaders();
	headers.Add("Content-Type", "application/json;charset=utf-8");
	headers.Add("Accept-Language", LanguageManager::GetLanguageKey());
	return headers;
}

TSharedPtr<FJsonObject> XDGNet::CommonParameters()
{
	TSharedPtr<FJsonObject> query = TDUHttpRequest::CommonParameters();
	
	query->SetStringField("clientId", DataStorage::LoadString(XDGDataStorageName::ClientId));

	query->SetStringField("sdkLang", LanguageManager::GetLanguageKey());
	query->SetStringField("lang", LanguageManager::GetLanguageKey());
	
	auto ipInfoModel = FIpInfoModel::GetLocalModel();
	if (ipInfoModel == nullptr)
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

	// query->SetStringField("appVer", FString::Printf(TEXT("%lld"), FDateTime::UtcNow().ToUnixTimestamp()));
	
	query->SetStringField("appVer", DeviceInfo::GetProjectVersion());
	query->SetStringField("appVerCode", DeviceInfo::GetProjectVersion());
	
	auto cfgMd = FInitConfigModel::GetLocalModel();
	query->SetStringField("appId", cfgMd == nullptr ? "" : cfgMd->configs.appId);

	
	// 		   {"mem", SystemInfo.systemMemorySize / 1024 + "GB"},
	// 		   {"mod", SystemInfo.deviceModel},
	// 		   {"brand", SystemInfo.graphicsDeviceVendor},

	return query;
}

bool XDGNet::ResetHeadersBeforeRequest()
{
	auto auth = GetMacToken();
	if (auth.Len() > 0)
	{
		this->Headers.Add("Authorization", auth);
	}
	return true;
}



FXDGError GenerateErrorInfo(const TSharedPtr<TDUHttpResponse>& response)
{
	FXDGError error;
	if (response->state == TDUHttpResponse::clientError)
	{
		error.code = TDUHttpResponse::clientError;
		error.msg = "request fail";
	} else if (response->state == TDUHttpResponse::serverError)
	{
		error.code = TDUHttpResponse::serverError;
		error.msg = "server error";
	} else if (response->state == TDUHttpResponse::networkError)
	{
		error.code = TDUHttpResponse::networkError;
		error.msg = "network connection error";
	}
	return error;
}

template <typename StructType>
TSharedPtr<StructType> GenerateStructPtr(const TSharedPtr<TDUHttpResponse>& response)
{
	if (response != nullptr && response->state == TDUHttpResponse::success) {
		return JsonHelper::GetUStruct<StructType>(response->contentString);
	} else {
		return nullptr;
	}
}

template <typename StructType>
void PerfromCallBack(const TSharedPtr<TDUHttpResponse>& response, TFunction<void(TSharedPtr<StructType> model, FXDGError error)> callback)
{
	if (callback == nullptr)
	{
		return;
	}
	TSharedPtr<StructType> model = GenerateStructPtr<StructType>(response);
	FXDGError error = GenerateErrorInfo(response);
	if (model == nullptr && error.code == 0)
	{
		error.code = TDUHttpResponse::clientError;
		error.msg = "json parse error";
	}
	callback(model, error);
}

template <typename StructType>
void PerfromWrapperResponseCallBack(const TSharedPtr<TDUHttpResponse>& response, TFunction<void(TSharedPtr<StructType> model, FXDGError error)> callback)
{
	if (callback == nullptr)
	{
		return;
	}
	TSharedPtr<FXDGResponseModel> Wrapper;
	TSharedPtr<StructType> model;
	FXDGResponseModel::ParseJson(response->contentString, Wrapper, model);
	FXDGError error;
	if (Wrapper == nullptr)
	{
		error = GenerateErrorInfo(response);
	} else
	{
		error.code = Wrapper->code;
		error.msg = Wrapper->msg;
		error.detail = Wrapper->detail;
	}
	if (response->state == TDUHttpResponse::success)
	{
		callback(model, error);
	} else
	{
		callback(nullptr, error);
	}
}

template <typename StructType>
void PerfromResponseCallBack(const TSharedPtr<TDUHttpResponse>& response, TFunction<void(TSharedPtr<StructType> model, FXDGError error)> callback)
{
	if (callback == nullptr)
	{
		return;
	}
	TSharedPtr<FXDGResponseModel> Wrapper = JsonHelper::GetUStruct<StructType>(response->contentString);;
	FXDGError error;
	if (Wrapper == nullptr)
	{
		error = GenerateErrorInfo(response);
	} else
	{
		error.code = Wrapper->code;
		error.msg = Wrapper->msg;
		error.detail = Wrapper->detail;
	}

	if (response->state == TDUHttpResponse::success)
	{
		callback(StaticCastSharedPtr<StructType>(Wrapper), error);
	} else
	{
		callback(nullptr, error);
	}
}

FString XDGNet::GetMacToken() {
	auto tokenModel = FTokenModel::GetLocalModel();
	FString authToken;
	if (tokenModel == nullptr)
	{
		return authToken;
	}
	UrlParse parse(this->GetFinalUrl());
	FString timeStr = FString::Printf(TEXT("%lld"), FDateTime::UtcNow().ToUnixTimestamp());
	FString nonce = TDSHelper::GetRandomStr(5);
	FString md = this->Type == Get ? "GET" : "POST";

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


void XDGNet::RequestIpInfo(TFunction<void(TSharedPtr<FIpInfoModel> model, FXDGError error)> callback)
{
	const TSharedPtr<TDUHttpRequest> request = MakeShareable(new XDGNet());
	request->URL = IP_INFO;
	request->isPure = true;
	request->RepeatCount = 3;
	request->onCompleted.BindLambda([=](TSharedPtr<TDUHttpResponse> response) {
		PerfromCallBack(response, callback);
	});
	TDUHttpManager::Get().request(request);
}

void XDGNet::RequestConfig(TFunction<void(TSharedPtr<FInitConfigModel> model, FXDGError error)> callback)
{
	const TSharedPtr<TDUHttpRequest> request = MakeShareable(new XDGNet());
	request->URL = INIT_SDK_URL;
	request->onCompleted.BindLambda([=](TSharedPtr<TDUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, callback);
	});
	TDUHttpManager::Get().request(request);
}

void XDGNet::RequestKidToken(const TSharedPtr<FJsonObject>& paras, TFunction<void(TSharedPtr<FTokenModel> model, FXDGError error)> callback)
{
	const TSharedPtr<TDUHttpRequest> request = MakeShareable(new XDGNet());
	request->URL = XDG_COMMON_LOGIN;
	request->Parameters = paras;
	request->Type = Post;
	request->isPure = true;
	request->Headers = request->CommonHeaders();
	request->PostUrlParameters = request->CommonParameters();
	request->onCompleted.BindLambda([=](TSharedPtr<TDUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, callback);
	});
	TDUHttpManager::Get().request(request);
}

void XDGNet::RequestUserInfo(TFunction<void(TSharedPtr<FXDGUser> model, FXDGError error)> callback)
{
	const TSharedPtr<TDUHttpRequest> request = MakeShareable(new XDGNet());
	request->URL = XDG_USER_PROFILE;
	request->onCompleted.BindLambda([=](TSharedPtr<TDUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, callback);
	});
	TDUHttpManager::Get().request(request);
}

void XDGNet::RequestSyncToken(TFunction<void(TSharedPtr<FSyncTokenModel> model, FXDGError error)> callback)
{
	const TSharedPtr<TDUHttpRequest> request = MakeShareable(new XDGNet());
	request->URL = XDG_LOGIN_SYN;
	request->Type = Post;
	request->isPure = true;
	request->Headers = request->CommonHeaders();
	request->PostUrlParameters = request->CommonParameters();
	request->onCompleted.BindLambda([=](TSharedPtr<TDUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, callback);
	});
	TDUHttpManager::Get().request(request);
}

void XDGNet::RequestPrivacyTxt(const FString& Url, TFunction<void(FString Txt)> callback)
{
	const TSharedPtr<TDUHttpRequest> request = MakeShareable(new XDGNet());
	request->URL = Url;
	// request->isPure = true;
	request->onCompleted.BindLambda([=](TSharedPtr<TDUHttpResponse> response) {
		callback(response->contentString);
	});
	TDUHttpManager::Get().request(request);
}

void XDGNet::RequestBindList(TFunction<void(TSharedPtr<FXDGBindResponseModel> Model, FXDGError Error)> Callback)
{
	const TSharedPtr<XDGNet> request = MakeShareable(new XDGNet());
	request->URL = XDG_BIND_LIST;
	request->onCompleted.BindLambda([=](TSharedPtr<TDUHttpResponse> response) {
		PerfromResponseCallBack(response, Callback);
	});
	TDUHttpManager::Get().request(request);
}

void XDGNet::Bind(const TSharedPtr<FJsonObject>& Paras,
	TFunction<void(TSharedPtr<FXDGResponseModel> Model, FXDGError Error)> Callback)
{
	const TSharedPtr<TDUHttpRequest> request = MakeShareable(new XDGNet());
	request->URL = XDG_BIND_INTERFACE;
	request->Parameters = Paras;
	request->Type = Post;
	request->isPure = true;
	request->Headers = request->CommonHeaders();
	request->PostUrlParameters = request->CommonParameters();
	request->onCompleted.BindLambda([=](TSharedPtr<TDUHttpResponse> response) {
		PerfromResponseCallBack(response, Callback);
	});
	TDUHttpManager::Get().request(request);
}

void XDGNet::Unbind(int LoginType, TFunction<void(TSharedPtr<FXDGResponseModel> Model, FXDGError Error)> Callback)
{
	const TSharedPtr<TDUHttpRequest> request = MakeShareable(new XDGNet());
	request->URL = XDG_UNBIND_INTERFACE;
	request->Parameters->SetNumberField("type", LoginType);
	request->Type = Post;
	request->isPure = true;
	request->Headers = request->CommonHeaders();
	request->PostUrlParameters = request->CommonParameters();
	request->onCompleted.BindLambda([=](TSharedPtr<TDUHttpResponse> response) {
		PerfromResponseCallBack(response, Callback);
	});
	TDUHttpManager::Get().request(request);
}
