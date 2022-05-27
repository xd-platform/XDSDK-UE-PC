#include "XUNet.h"
#include "JsonObjectConverter.h"
#include "XULanguageManager.h"
#include "XUStorage.h"
#include "XUIpInfoModel.h"
#include "TUDeviceInfo.h"
#include "TUJsonHelper.h"
#include "TUCrypto.h"
#include "TUHelper.h"
#include "XUTokenModel.h"
#include "URLParser.h"
#include "XUResponseModel.h"
#include "XDGSDK.h"

// public readonly static string BASE_URL = "https://test-xdsdk-intnl-6.xd.com"; //测试
static FString BASE_URL = "https://xdsdk-intnl-6.xd.com"; //正式
// static NSString *TDSG_GLOBAL_SDK_DOMAIN = @"https://xdsdk-6.xd.cn";

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

XUNet::XUNet()
{
	Form = Json;
}


TMap<FString, FString> XUNet::CommonHeaders()
{
	TMap<FString, FString> headers = TUHttpRequest::CommonHeaders();
	headers.Add("Content-Type", "application/json;charset=utf-8");
	headers.Add("Accept-Language", XULanguageManager::GetLanguageKey());
	return headers;
}

TSharedPtr<FJsonObject> XUNet::CommonParameters()
{
	TSharedPtr<FJsonObject> query = TUHttpRequest::CommonParameters();
	
	query->SetStringField("clientId", TUDataStorage<FXUStorage>::LoadString(FXUStorage::ClientId));

	query->SetStringField("sdkLang", XULanguageManager::GetLanguageKey());
	query->SetStringField("lang", XULanguageManager::GetLanguageKey());
	
	auto ipInfoModel = FXUIpInfoModel::GetLocalModel();
	if (ipInfoModel == nullptr)
	{
		ipInfoModel = MakeShareable(new FXUIpInfoModel);
	}
	query->SetStringField("loc", ipInfoModel->country_code);
	query->SetStringField("city", ipInfoModel->city);
	query->SetStringField("timeZone", ipInfoModel->timeZone);
	query->SetStringField("countryCode", ipInfoModel->country_code);
	
	query->SetStringField("locationInfoType", "ip");
	query->SetStringField("chn", "PC");

	query->SetStringField("sdkVer", XDUESDK_VERSION);

	query->SetStringField("did", TUDeviceInfo::GetLoginId());
	query->SetStringField("pt", TUDeviceInfo::GetPlatform());
	query->SetStringField("pkgName", TUDeviceInfo::GetProjectName());
	query->SetStringField("os", TUDeviceInfo::GetOSVersion());
	query->SetStringField("res", FString::Printf(TEXT("%d_%d"), TUDeviceInfo::GetScreenWidth(), TUDeviceInfo::GetScreenHeight()));
	
	query->SetStringField("time", FString::Printf(TEXT("%lld"), FDateTime::UtcNow().ToUnixTimestamp()));

	// query->SetStringField("appVer", FString::Printf(TEXT("%lld"), FDateTime::UtcNow().ToUnixTimestamp()));
	
	query->SetStringField("appVer", TUDeviceInfo::GetProjectVersion());
	query->SetStringField("appVerCode", TUDeviceInfo::GetProjectVersion());
	
	auto cfgMd = FXUInitConfigModel::GetLocalModel();
	query->SetStringField("appId", cfgMd == nullptr ? "" : cfgMd->configs.appId);

	
	// 		   {"mem", SystemInfo.systemMemorySize / 1024 + "GB"},
	// 		   {"mod", SystemInfo.deviceModel},
	// 		   {"brand", SystemInfo.graphicsDeviceVendor},

	return query;
}

bool XUNet::ResetHeadersBeforeRequest()
{
	auto auth = GetMacToken();
	if (auth.Len() > 0)
	{
		this->Headers.Add("Authorization", auth);
	}
	return true;
}



FXUError XUNet::GenerateErrorInfo(const TSharedPtr<TUHttpResponse>& response)
{
	FXUError error;
	if (response->state == TUHttpResponse::clientError)
	{
		error.code = TUHttpResponse::clientError;
		error.msg = "request fail";
	} else if (response->state == TUHttpResponse::serverError)
	{
		error.code = TUHttpResponse::serverError;
		error.msg = "server error";
	} else if (response->state == TUHttpResponse::networkError)
	{
		error.code = TUHttpResponse::networkError;
		error.msg = "network connection error";
	}
	return error;
}

template <typename StructType>
TSharedPtr<StructType> GenerateStructPtr(const TSharedPtr<TUHttpResponse>& response)
{
	if (response != nullptr && response->state == TUHttpResponse::success) {
		return TUJsonHelper::GetUStruct<StructType>(response->contentString);
	} else {
		return nullptr;
	}
}

template <typename StructType>
void PerfromCallBack(const TSharedPtr<TUHttpResponse>& response, TFunction<void(TSharedPtr<StructType> model, FXUError error)> callback)
{
	if (callback == nullptr)
	{
		return;
	}
	TSharedPtr<StructType> model = GenerateStructPtr<StructType>(response);
	FXUError error = XUNet::GenerateErrorInfo(response);
	if (model == nullptr && error.code == 0)
	{
		error.code = TUHttpResponse::clientError;
		error.msg = "json parse error";
	}
	callback(model, error);
}

template <typename StructType>
void PerfromWrapperResponseCallBack(const TSharedPtr<TUHttpResponse>& response, TFunction<void(TSharedPtr<StructType> model, FXUError error)> callback)
{
	if (callback == nullptr)
	{
		return;
	}
	TSharedPtr<FXUResponseModel> Wrapper;
	TSharedPtr<StructType> model;
	FXUResponseModel::ParseJson(response->contentString, Wrapper, model);
	FXUError error;
	if (Wrapper == nullptr)
	{
		error = XUNet::GenerateErrorInfo(response);
	} else
	{
		error.code = Wrapper->code;
		error.msg = Wrapper->msg;
		error.detail = Wrapper->detail;
	}
	if (response->state == TUHttpResponse::success)
	{
		callback(model, error);
	} else
	{
		callback(nullptr, error);
	}
}

template <typename StructType>
void PerfromResponseCallBack(const TSharedPtr<TUHttpResponse>& response, TFunction<void(TSharedPtr<StructType> model, FXUError error)> callback)
{
	if (callback == nullptr)
	{
		return;
	}
	TSharedPtr<FXUResponseModel> Wrapper = TUJsonHelper::GetUStruct<StructType>(response->contentString);;
	FXUError error;
	if (Wrapper == nullptr)
	{
		error = XUNet::GenerateErrorInfo(response);
	} else
	{
		error.code = Wrapper->code;
		error.msg = Wrapper->msg;
		error.detail = Wrapper->detail;
	}

	if (response->state == TUHttpResponse::success)
	{
		callback(StaticCastSharedPtr<StructType>(Wrapper), error);
	} else
	{
		callback(nullptr, error);
	}
}

FString XUNet::GetMacToken() {
	auto tokenModel = FXUTokenModel::GetLocalModel();
	FString authToken;
	if (tokenModel == nullptr)
	{
		return authToken;
	}
	auto Parse = TUCommon::FURL_RFC3986();
	Parse.Parse(this->GetFinalUrl());
	FString timeStr = FString::Printf(TEXT("%lld"), FDateTime::UtcNow().ToUnixTimestamp());
	FString nonce = TUHelper::GetRandomStr(5);
	FString md = this->Type == Get ? "GET" : "POST";

	FString pathAndQuery = Parse.GetPath();
	if (Parse.GetQuery().Len() > 0)
	{
		pathAndQuery += "?" + Parse.GetQuery();
	}
	FString domain = Parse.GetHost().ToLower();
	FString port = Parse.GetPort();

	FString dataStr = timeStr + "\n" + nonce + "\n" + md + "\n" + pathAndQuery + "\n" + domain + "\n" + port + "\n";
	auto sha1 = TUCrypto::HmacSHA1(TUCrypto::UTF8Encode(dataStr), TUCrypto::UTF8Encode(tokenModel->macKey));
	FString mac = TUCrypto::Base64Encode(sha1);
	
	authToken = FString::Printf(TEXT("MAC id=\"%s\",ts=\"%s\",nonce=\"%s\",mac=\"%s\""), *tokenModel->kid, *timeStr, *nonce, *mac);
	return authToken;
}


void XUNet::RequestIpInfo(TFunction<void(TSharedPtr<FXUIpInfoModel> model, FXUError error)> callback)
{
	const TSharedPtr<TUHttpRequest> request = MakeShareable(new XUNet());
	request->URL = IP_INFO;
	request->isPure = true;
	request->RepeatCount = 3;
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromCallBack(response, callback);
	});
	TUHttpManager::Get().request(request);
}

void XUNet::RequestConfig(TFunction<void(TSharedPtr<FXUInitConfigModel> model, FXUError error)> callback)
{
	const TSharedPtr<TUHttpRequest> request = MakeShareable(new XUNet());
	request->URL = INIT_SDK_URL;
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, callback);
	});
	TUHttpManager::Get().request(request);
}

void XUNet::RequestKidToken(const TSharedPtr<FJsonObject>& paras, TFunction<void(TSharedPtr<FXUTokenModel> model, FXUError error)> callback)
{
	const TSharedPtr<TUHttpRequest> request = MakeShareable(new XUNet());
	request->URL = XDG_COMMON_LOGIN;
	request->Parameters = paras;
	request->Type = Post;
	request->isPure = true;
	request->Headers = request->CommonHeaders();
	request->PostUrlParameters = request->CommonParameters();
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, callback);
	});
	TUHttpManager::Get().request(request);
}

void XUNet::RequestUserInfo(TFunction<void(TSharedPtr<FXUUser> model, FXUError error)> callback)
{
	const TSharedPtr<TUHttpRequest> request = MakeShareable(new XUNet());
	request->URL = XDG_USER_PROFILE;
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, callback);
	});
	TUHttpManager::Get().request(request);
}

void XUNet::RequestSyncToken(TFunction<void(TSharedPtr<FXUSyncTokenModel> model, FXUError error)> callback)
{
	const TSharedPtr<TUHttpRequest> request = MakeShareable(new XUNet());
	request->URL = XDG_LOGIN_SYN;
	request->Type = Post;
	request->isPure = true;
	request->Headers = request->CommonHeaders();
	request->PostUrlParameters = request->CommonParameters();
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, callback);
	});
	TUHttpManager::Get().request(request);
}

void XUNet::RequestPrivacyTxt(const FString& Url, TFunction<void(FString Txt)> callback)
{
	const TSharedPtr<TUHttpRequest> request = MakeShareable(new XUNet());
	request->URL = Url;
	// request->isPure = true;
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		callback(response->contentString);
	});
	TUHttpManager::Get().request(request);
}

void XUNet::RequestBindList(TFunction<void(TSharedPtr<FXUBindResponseModel> Model, FXUError Error)> Callback)
{
	const TSharedPtr<XUNet> request = MakeShareable(new XUNet());
	request->URL = XDG_BIND_LIST;
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromResponseCallBack(response, Callback);
	});
	TUHttpManager::Get().request(request);
}

void XUNet::Bind(const TSharedPtr<FJsonObject>& Paras,
	TFunction<void(TSharedPtr<FXUResponseModel> Model, FXUError Error)> Callback)
{
	const TSharedPtr<TUHttpRequest> request = MakeShareable(new XUNet());
	request->URL = XDG_BIND_INTERFACE;
	request->Parameters = Paras;
	request->Type = Post;
	request->isPure = true;
	request->Headers = request->CommonHeaders();
	request->PostUrlParameters = request->CommonParameters();
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromResponseCallBack(response, Callback);
	});
	TUHttpManager::Get().request(request);
}

void XUNet::Unbind(int LoginType, TFunction<void(TSharedPtr<FXUResponseModel> Model, FXUError Error)> Callback)
{
	const TSharedPtr<TUHttpRequest> request = MakeShareable(new XUNet());
	request->URL = XDG_UNBIND_INTERFACE;
	request->Parameters->SetNumberField("type", LoginType);
	request->Type = Post;
	request->isPure = true;
	request->Headers = request->CommonHeaders();
	request->PostUrlParameters = request->CommonParameters();
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromResponseCallBack(response, Callback);
	});
	TUHttpManager::Get().request(request);
}

void XUNet::CheckPay(TFunction<void(TSharedPtr<FXUPayCheckResponseModel> Model, FXUError Error)> Callback)
{
	const TSharedPtr<XUNet> request = MakeShareable(new XUNet());
	request->URL = XDG_PAYBACK_LIST;
	request->Parameters->SetStringField("userId", FXUUser::GetLocalModel()->userId);
	request->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
		PerfromWrapperResponseCallBack(response, Callback);
	});
	TUHttpManager::Get().request(request);
}