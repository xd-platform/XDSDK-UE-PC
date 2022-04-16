#include "XDGNet.h"
#include "JsonObjectConverter.h"
#include "Model/IpInfoModel.h"

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


void XDGNet::RequestIpInfo()
{
	const TSharedPtr<TDSHttpRequest> request = MakeShareable(new XDGNet());
	request->URL = IP_INFO;
	request->onCompleted = [](TSharedPtr<TDSHttpResponse> response)
	{
		TSharedPtr<FIpInfoModel> model = MakeShareable(new FIpInfoModel);
		FJsonObjectConverter::JsonObjectStringToUStruct(response->contentString, model.Get());
		UE_LOG(LogTemp, Warning, TEXT("%s, %s, %s, %s, %s"), *model->city, *model->country, *model->country_code, *model->latitude, *model->longitude);
	};
	TDSHttpManager::Get().request(request);

}

/*Assigned function on successfull http call*/
// void TDSHttpRequest::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
// {
// 	//Create a pointer to hold the json serialized data
// 	TSharedPtr<FJsonObject> JsonObject;
//
// 	//Create a reader pointer to read the json data
// 	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
//
// 	//Deserialize the json data given Reader and the actual object to deserialize
// 	if (FJsonSerializer::Deserialize(Reader, JsonObject))
// 	{
// 		//Get the value of the json object by field name
// 		int32 recievedInt = JsonObject->GetIntegerField("customInt");
//
// 		//Output it to the engine
// 		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, FString::FromInt(recievedInt));
// 	}
// }
