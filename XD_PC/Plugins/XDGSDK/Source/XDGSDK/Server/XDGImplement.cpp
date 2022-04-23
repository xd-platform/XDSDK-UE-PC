#include "XDGImplement.h"
#include "DataStorageName.h"
#include "DeviceInfo.h"
#include "JsonHelper.h"
#include "LanguageManager.h"
#include "TapBootstrapAPI.h"
#include "TapConfig.h"
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
	FTapConfig Config;
	Config.clientID = tapCfg.clientId;
	Config.clientToken = tapCfg.clientToken;
	Config.serverURL = tapCfg.serverUrl;
	Config.regionType = RegionType::IO;
	Config.dbConfig.enable = tapCfg.enableTapDB;
	Config.dbConfig.channel = tapCfg.tapDBChannel;
	Config.dbConfig.gameVersion = DeviceInfo::GetProjectVersion();
	UTapBootstrap::Init(Config);
	
	if (resultBlock) { resultBlock(true, msg);}
}


void XDGImplement::LoginByType(LoginType loginType,
	TFunction<void(TSharedPtr<FXDGUser> user)> resultBlock, TFunction<void(FXDGError error)> ErrorBlock)
{
	auto lmd = LanguageManager::GetCurrentModel();
	if (loginType == LoginType::Default)
	{
		
	} else
	{
		GetLoginParam(loginType,
		[=](TSharedPtr<FJsonObject> paras)
		{
			RequestKidToken(paras,
			[=](TSharedPtr<FTokenModel> kidToken)
			{
				RequestUserInfo(false,
				[=](TSharedPtr<FXDGUser> user)
				{
					AsyncNetworkTdsUser(user->userId,
					[=](FString SessionToken)
					{
						DataStorage::SaveStruct(DataStorageName::UserInfo, user);
						resultBlock(user);
					}, ErrorBlock);
				}, ErrorBlock);
			}, ErrorBlock);
		}, ErrorBlock);
	}
}

void XDGImplement::GetLoginParam(LoginType loginType,
	TFunction<void(TSharedPtr<FJsonObject> paras)> resultBlock, TFunction<void(FXDGError error)> ErrorBlock)
{
	if (loginType == LoginType::Guest)
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		JsonObject->SetNumberField("type", (int)loginType);
		JsonObject->SetStringField("token", DeviceInfo::GetLoginId());
		resultBlock(JsonObject);
	} else if(loginType == LoginType::TapTap)
	{
		
	} else
	{
		ErrorBlock(FXDGError("No Login Param"));
	}
}

void XDGImplement::RequestKidToken(TSharedPtr<FJsonObject> paras,
	TFunction<void(TSharedPtr<FTokenModel> kidToken)> resultBlock, TFunction<void(FXDGError error)> ErrorBlock)
{
	XDGNet::RequestKidToken(paras,
		[=](TSharedPtr<FTokenModel> kidToken, FXDGError error)
		{
			if (error.code == Success && kidToken != nullptr)
			{
				DataStorage::SaveStruct(DataStorageName::TokenInfo, kidToken);
				resultBlock(kidToken);
			} else
			{
				auto localToken = DataStorage::LoadStruct<FTokenModel>(DataStorageName::TokenInfo);
				if (localToken == nullptr)
				{
					ErrorBlock(error);
				} else
				{
					resultBlock(kidToken);
				}
			}
		}
	);
}

void XDGImplement::RequestUserInfo(bool saveToLocal,
	TFunction<void(TSharedPtr<FXDGUser> model)> callback, TFunction<void(FXDGError error)> ErrorBlock)
{
	XDGNet::RequestUserInfo(
	[=](TSharedPtr<FXDGUser> user, FXDGError error)
	{
		if (error.code == Success && user != nullptr)
		{
			if (saveToLocal)
			{
				DataStorage::SaveStruct(DataStorageName::UserInfo, user);
			}
			callback(user);
		} else
		{
			auto localUser = DataStorage::LoadStruct<FXDGUser>(DataStorageName::UserInfo);
			if (localUser == nullptr)
			{
				ErrorBlock(error);
			} else
			{
				callback(localUser);
			}
		}
	});
	
}

void XDGImplement::AsyncNetworkTdsUser(const FString& userId,
	TFunction<void(FString SessionToken)> callback, TFunction<void(FXDGError error)> ErrorBlock)
{
	XDGNet::RequestSyncToken(
	[=](TSharedPtr<FSyncTokenModel> model, FXDGError error)
	{
		if (error.code == Success && model != nullptr)
		{
			DataStorage::SaveStruct(DataStorageName::SessionTokenKey, model);

			// LCUser lcUser = LCObject.CreateWithoutData(LCUser.CLASS_NAME, userId) as LCUser;
			// lcUser.SessionToken = md.data.sessionToken;
			// await lcUser.SaveToLocal();
	
			callback(model->sessionToken);
		} else
		{
			auto localModel = DataStorage::LoadStruct<FSyncTokenModel>(DataStorageName::SessionTokenKey);
			if (localModel == nullptr)
			{
				ErrorBlock(error);
			} else
			{
				// LCUser lcUser = LCObject.CreateWithoutData(LCUser.CLASS_NAME, userId) as LCUser;
				// lcUser.SessionToken = token;
				// await lcUser.SaveToLocal();
				callback(localModel->sessionToken);
			}
		}
	}
	);
}


