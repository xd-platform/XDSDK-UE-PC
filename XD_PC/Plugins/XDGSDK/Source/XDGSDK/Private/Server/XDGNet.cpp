#include "XDGNet.h"
#include "JsonObjectConverter.h"


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


void XDGNet::RequestConfig()
{
	const TSharedPtr<TDSHttpRequest> request = MakeShareable(new XDGNet());
	request->URL = INIT_SDK_URL;
	TDSHttpManager::Get().request(request);
}

void XDGNet::RequestIpInfo(TFunction<void(bool success, TSharedPtr<FIpInfoModel> model)> callback)
{
	const TSharedPtr<TDSHttpRequest> request = MakeShareable(new XDGNet());
	request->URL = IP_INFO;
	request->repeatCount = 3;
	request->onCompleted.BindLambda([=](TSharedPtr<TDSHttpResponse> response) {
		if (callback == nullptr)
		{
			return;
		}
		if (response->state == TDSHttpResponse::success) {
			TSharedPtr<FIpInfoModel> model = MakeShareable(new FIpInfoModel);
			FJsonObjectConverter::JsonObjectStringToUStruct(response->contentString, model.Get());
			callback(true, model);
		} else {
			callback(false, nullptr);
		}
		
	});
	TDSHttpManager::Get().request(request);
}




