#include "XDGImplement.h"
#include "DataStorageName.h"

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
		if (model == nullptr)
		{
			// auto cModel = 
			// TSharedPtr<FIpInfoModel> infoModel = DataStorage::LoadStruct<FIpInfoModel>(DataStorageName::IpInfo);
			// if (resultBlock) { resultBlock(infoModel, error.msg);}
		} else
		{
			// DataStorage::SaveStruct(DataStorageName::IpInfo, model, true);
			// if (resultBlock) { resultBlock(model, "success");}
		}
	}
	);
}
