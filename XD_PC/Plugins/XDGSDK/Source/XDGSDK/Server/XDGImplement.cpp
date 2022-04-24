#include "XDGImplement.h"
#include "XDGDataStorageName.h"
#include "DeviceInfo.h"
#include "JsonHelper.h"
#include "LanguageManager.h"
#include "TapBootstrapAPI.h"
#include "TapConfig.h"
#include "XDGSDK.h"
#include "XDGSDK/UI/XDGPrivacyWidget.h"

static int Success = 200;

void XDGImplement::GetIpInfo(TFunction<void(TSharedPtr<FIpInfoModel> model, FString msg)> resultBlock)
{
	XDGNet::RequestIpInfo(
		[=] (TSharedPtr<FIpInfoModel> model, FXDGError error)
		{
			if (model == nullptr)
			{
				TSharedPtr<FIpInfoModel> infoModel = FIpInfoModel::GetLocalModel();
				if (resultBlock) { resultBlock(infoModel, error.msg);}
			} else
			{
				model->SaveToLocal();
				if (resultBlock) { resultBlock(model, "success");}
			}
		}
	);
}

void XDGImplement::InitSDK(FString sdkClientId, TFunction<void(bool successed, FString msg)> resultBlock)
{
	DataStorage::SaveString(XDGDataStorageName::ClientId, sdkClientId, false);
	XDGNet::RequestConfig(
	[=] (TSharedPtr<FInitConfigModel> model, FXDGError error)
	{
		if (model != nullptr && error.code == Success)
		{
			model->SaveToLocal();
			InitBootstrap(model, resultBlock, error.msg);
		} else
		{
			InitBootstrap(FInitConfigModel::GetLocalModel(), resultBlock, error.msg);
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
						CheckPrivacyAlert(
						[=]()
						{
							user->SaveToLocal();
							resultBlock(user);
						});
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
				kidToken->SaveToLocal();
				resultBlock(kidToken);
			} else
			{
				auto localToken = FTokenModel::GetLocalModel();
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
				user->SaveToLocal();
			}
			callback(user);
		} else
		{
			auto localUser = FXDGUser::GetLocalModel();
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
			model->SaveToLocal();

			// LCUser lcUser = LCObject.CreateWithoutData(LCUser.CLASS_NAME, userId) as LCUser;
			// lcUser.SessionToken = md.data.sessionToken;
			// await lcUser.SaveToLocal();
	
			callback(model->sessionToken);
		} else
		{
			auto localModel = FSyncTokenModel::GetLocalModel();
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

void XDGImplement::CheckPrivacyAlert(TFunction<void()> Callback)
{
	if (FInitConfigModel::CanShowPrivacyAlert())
	{
		UXDGPrivacyWidget::ShowPrivacy(
		[=](bool result)
		{
			if (result)
			{
				Callback();
			}
		});
	} else
	{
		Callback();
	}
}


