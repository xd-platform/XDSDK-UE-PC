#include "XDGImplement.h"
#include "Tools/DataStorageName.h"

void XDGImplement::GetIpInfo(TFunction<void(TSharedPtr<FIpInfoModel> model, FString msg)> resultBlock)
{
	XDGNet::RequestIpInfo(
		[=] (TSharedPtr<FIpInfoModel> model, FXDGError error)
		{
			if (model == nullptr)
			{
				TSharedPtr<FIpInfoModel> infoModel = DataStorage::LoadStruct<FIpInfoModel>(DataStorageName_IpInfo);
				if (resultBlock) { resultBlock(infoModel, error.msg);}
			} else
			{
				DataStorage::SaveStruct(DataStorageName_IpInfo, model, true);
				if (resultBlock) { resultBlock(model, "success");}
			}
		}
	);
}

void XDGImplement::InitSDK(FString sdkClientId, TFunction<void(bool successed, FString msg)> resultBlock)
{
	DataStorage::SaveString(DataStorageName_ClientID, sdkClientId, false);
}
