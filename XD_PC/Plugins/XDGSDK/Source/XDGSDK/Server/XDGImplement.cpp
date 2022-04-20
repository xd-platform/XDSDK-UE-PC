#include "XDGImplement.h"
#include "DataStorageName.h"
#include "JsonHelper.h"
#include "XDGSDK.h"

static int Success = 200;

void XDGImplement::GetIpInfo(TFunction<void(TSharedPtr<FIpInfoModel> model, FString msg)> resultBlock)
{
	XDGNet::RequestIpInfo(
		[=] (TSharedPtr<FIpInfoModel> model, FXDGError error)
		{
			if (model == nullptr)
			{
				TSharedPtr<FIpInfoModel> infoModel = DataStorage::LoadStruct<FIpInfoModel>(DataStorageName::IpInfo);
				if (resultBlock) { resultBlock(infoModel, error.msg);}
			} else
			{
				DataStorage::SaveStruct(DataStorageName::IpInfo, model, true);
				if (resultBlock) { resultBlock(model, "success");}
			}
		}
	);
}

void XDGImplement::InitSDK(FString sdkClientId, TFunction<void(bool successed, FString msg)> resultBlock)
{
	DataStorage::SaveString(DataStorageName::ClientId, sdkClientId, false);
	XDGNet::RequestConfig(
	[=] (TSharedPtr<FInitConfigModel> model, FXDGError error)
	{
		if (model != nullptr && error.code == Success)
		{
			DataStorage::SaveStruct(DataStorageName::InitConfig, model);
			InitBootstrap(model, resultBlock, error.msg);
		} else
		{
			InitBootstrap(DataStorage::LoadStruct<FInitConfigModel>(DataStorageName::InitConfig), resultBlock, error.msg);
		}
	}
	);
}

void XDGImplement::InitBootstrap(const TSharedPtr<FInitConfigModel>& model, TFunction<void(bool successed, FString msg)> resultBlock, const FString& msg)
{
	if (model == nullptr)
	{
		if (resultBlock) { resultBlock(false, msg);}
		return;
	}
	auto tapCfg = model->configs.tapSdkConfig;
	// TapLogin.Init(tapCfg.clientId, false, false);
	// var config = new TapConfig.Builder()
	// 	.ClientID(tapCfg.clientId)
	// 	.ClientToken(tapCfg.clientToken)
	// 	.ServerURL(tapCfg.serverUrl)
	// 	.RegionType(RegionType.IO) //IO：海外 
	// 	.TapDBConfig(tapCfg.enableTapDB, tapCfg.tapDBChannel, Application.version)
	// 	.ConfigBuilder();
	// TapBootstrap.Init(config);
	if (resultBlock) { resultBlock(true, msg);}
}


// private static void InitBootstrap(InitConfigModel infoMd, Action<bool, string> callback, string msg){

