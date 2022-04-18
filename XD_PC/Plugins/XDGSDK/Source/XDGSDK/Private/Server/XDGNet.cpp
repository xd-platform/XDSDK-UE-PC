#include "XDGNet.h"
#include "JsonObjectConverter.h"
#include "LanguageManager.h"


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
		TSharedPtr<StructType> model = MakeShareable(new StructType);
		FJsonObjectConverter::JsonObjectStringToUStruct(response->contentString, model.Get());
		return model;
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
	callback(model, error);
}


void XDGNet::RequestConfig()
{
	const TSharedPtr<TDSHttpRequest> request = MakeShareable(new XDGNet());
	request->URL = INIT_SDK_URL;
	TDSHttpManager::Get().request(request);
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




