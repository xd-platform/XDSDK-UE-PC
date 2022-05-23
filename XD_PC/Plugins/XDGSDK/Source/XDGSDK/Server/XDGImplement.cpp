#include "XDGImplement.h"
#include "XDGStorage.h"
#include "TUDeviceInfo.h"
#include "TUJsonHelper.h"
#include "LanguageManager.h"
#include "TapBootstrapAPI.h"
#include "TapUELogin.h"
#include "TUCommonConfig.h"
#include "TUHelper.h"
#include "TUHUD.h"
#include "URLParser.h"
#include "XDGSDK.h"
#include "XDGSDK/UI/XDGPrivacyWidget.h"

static int Success = 200;

void XDGImplement::GetIpInfo(TFunction<void(TSharedPtr<FIpInfoModel> model, FString msg)> resultBlock) {
	XDGNet::RequestIpInfo([=](TSharedPtr<FIpInfoModel> model, FXDGError error) {
		if (model == nullptr) {
			TSharedPtr<FIpInfoModel> infoModel = FIpInfoModel::GetLocalModel();
			if (resultBlock) { resultBlock(infoModel, error.msg); }
		}
		else {
			model->SaveToLocal();
			if (resultBlock) { resultBlock(model, "success"); }
		}
	});
}

void XDGImplement::InitSDK(FString sdkClientId, TFunction<void(bool successed, FString msg)> resultBlock) {
	TUDataStorage<FXDGStorage>::SaveString(FXDGStorage::ClientId, sdkClientId, false);
	XDGNet::RequestConfig([=](TSharedPtr<FInitConfigModel> model, FXDGError error) {
		if (model != nullptr && error.code == Success) {
			model->SaveToLocal();
			InitBootstrap(model, resultBlock, error.msg);
		}
		else {
			InitBootstrap(FInitConfigModel::GetLocalModel(), resultBlock, error.msg);
		}
	});
}

void XDGImplement::InitBootstrap(const TSharedPtr<FInitConfigModel>& model,
                                 TFunction<void(bool successed, FString msg)> resultBlock, const FString& msg) {
	if (model == nullptr) {
		if (resultBlock) { resultBlock(false, msg); }
		return;
	}
	auto tapCfg = model->configs.tapSdkConfig;
	FTUCommonConfig Config;
	Config.clientID = tapCfg.clientId;
	Config.clientToken = tapCfg.clientToken;
	Config.serverURL = tapCfg.serverUrl;
	Config.regionType = RegionType::IO;
	Config.dbConfig.enable = tapCfg.enableTapDB;
	Config.dbConfig.channel = tapCfg.tapDBChannel;
	Config.dbConfig.gameVersion = TUDeviceInfo::GetProjectVersion();
	UTapBootstrap::Init(Config);

	if (resultBlock) { resultBlock(true, msg); }
}


void XDGImplement::LoginByType(XDLoginType loginType,
                               TFunction<void(TSharedPtr<FXDGUser> user)> resultBlock,
                               TFunction<void(FXDGError error)> ErrorBlock) {
	auto lmd = LanguageManager::GetCurrentModel();
	if (loginType == XDLoginType::Default) {
		auto localUser = FXDGUser::GetLocalModel();
		if (localUser.IsValid()) {
			RequestUserInfo(true, [=](TSharedPtr<FXDGUser> user) {
				AsyncLocalTdsUser(user->userId, FSyncTokenModel::GetLocalModel()->sessionToken);
				resultBlock(user);
			}, ErrorBlock);
		}
		else {
			ErrorBlock(FXDGError(lmd->tds_login_failed));
		}
	}
	else {
		GetLoginParam(loginType, [=](TSharedPtr<FJsonObject> paras) {
			UTUHUD::ShowWait();
			TFunction<void(FXDGError error)> ErrorCallBack = [=](FXDGError error) {
				UTUHUD::Dismiss();
				if (ErrorBlock) {
					ErrorBlock(error);
				}
			};
			RequestKidToken(paras, [=](TSharedPtr<FTokenModel> kidToken) {
				RequestUserInfo(false, [=](TSharedPtr<FXDGUser> user) {
					AsyncNetworkTdsUser(user->userId, [=](FString SessionToken) {
						UTUHUD::Dismiss();
						CheckPrivacyAlert([=]() {
							user->SaveToLocal();
							resultBlock(user);
						});
					}, ErrorCallBack);
				}, ErrorCallBack);
			}, ErrorCallBack);
		}, ErrorBlock);
	}
}

void XDGImplement::GetLoginParam(XDLoginType loginType,
                                 TFunction<void(TSharedPtr<FJsonObject> paras)> resultBlock,
                                 TFunction<void(FXDGError error)> ErrorBlock) {
	if (loginType == XDLoginType::Guest) {
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		JsonObject->SetNumberField("type", (int)loginType);
		JsonObject->SetStringField("token", TUDeviceInfo::GetLoginId());
		resultBlock(JsonObject);
	}
	else if (loginType == XDLoginType::TapTap) {
		RequestTapToken(
			[=](FTUAccessToken AccessToken) {
				TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
				JsonObject->SetNumberField("type", (int)loginType);
				JsonObject->SetStringField("token", AccessToken.kid);
				JsonObject->SetStringField("secret", AccessToken.mac_key);
				resultBlock(JsonObject);
			}, ErrorBlock);
	}
	else {
		ErrorBlock(FXDGError("No Login Param"));
	}
}

void XDGImplement::CheckPay(TFunction<void(XDCheckPayType CheckType)> SuccessBlock,
                            TFunction<void(FXDGError Error)> FailBlock) {
	XDGNet::CheckPay([=](TSharedPtr<FXDIPayCheckResponseModel> Model, FXDGError Error) {
		if (Model.IsValid()) {
			bool hasIOS = false;
			bool hasAndroid = false;
			for (auto md : Model->list) {
				if (md.platform == 1) {
					hasIOS = true;
				}
				if (md.platform == 2) {
					hasAndroid = true;
				}
			}
			if (SuccessBlock) {
				if (hasIOS && hasAndroid) {
					SuccessBlock(iOSAndAndroid);
				}
				else if (hasIOS) {
					SuccessBlock(iOS);
				}
				else if (hasAndroid) {
					SuccessBlock(Android);
				}
				else {
					SuccessBlock(None);
				}
			}

		}
		else {
			if (FailBlock) {
				FailBlock(Error);
			}
		}
	});
}

FString XDGImplement::GetCustomerCenter(const FString& ServerId, const FString& RoleId, const FString& RoleName) {
	auto userMd = FXDGUser::GetLocalModel();
	auto cfgMd = FInitConfigModel::GetLocalModel();
	auto tkModel = FTokenModel::GetLocalModel();
	if (!userMd.IsValid() || !cfgMd.IsValid() || !tkModel.IsValid()) {
		return "";
	}
	
	TSharedPtr<FJsonObject> query = MakeShareable(new FJsonObject);
	query->SetStringField("client_id", TUDataStorage<FXDGStorage>::LoadString(FXDGStorage::ClientId));
	query->SetStringField("access_token", tkModel->kid);
	query->SetStringField("user_id", userMd->userId);
	query->SetStringField("server_id", ServerId);
	query->SetStringField("role_id", RoleId);
	query->SetStringField("role_name", RoleName);
	query->SetStringField("region", cfgMd->configs.region);
	query->SetStringField("sdk_ver", FXDGSDKModule::VersionName);
	query->SetStringField("sdk_lang", LanguageManager::GetCustomerCenterLang());
	query->SetStringField("app_ver", TUDeviceInfo::GetProjectVersion());
	query->SetStringField("app_ver_code", TUDeviceInfo::GetProjectVersion());
	query->SetStringField("res", FString::Printf(TEXT("%d_%d"), TUDeviceInfo::GetScreenWidth(), TUDeviceInfo::GetScreenHeight()));
	query->SetStringField("cpu", TUDeviceInfo::GetCPU());
	query->SetStringField("pt", TUDeviceInfo::GetPlatform());
	query->SetStringField("os", TUDeviceInfo::GetOSVersion());
	query->SetStringField("brand", TUDeviceInfo::GetGPU());
	query->SetStringField("game_name", TUDeviceInfo::GetProjectName());

	FString QueryStr = TUHelper::CombinParameters(query);
	FString UrlStr = cfgMd->configs.reportUrl;
	auto Parse = TUCommon::FURL_RFC3986();
	Parse.Parse(UrlStr);
	UrlStr = FString::Printf(TEXT("%s://%s?%s"), *Parse.GetScheme(), *Parse.GetHost(), *QueryStr);
	return UrlStr;
}

FString XDGImplement::GetPayUrl(const FString& ServerId, const FString& RoleId) {
	auto userMd = FXDGUser::GetLocalModel();
	auto cfgMd = FInitConfigModel::GetLocalModel();
	if (!userMd.IsValid() || !cfgMd.IsValid()) {
		return "";
	}
	
	TSharedPtr<FJsonObject> query = MakeShareable(new FJsonObject);

	query->SetStringField("serverId", ServerId);
	query->SetStringField("roleId", RoleId);
	query->SetStringField("region", cfgMd->configs.region);
	query->SetStringField("appId", cfgMd->configs.appId);
	query->SetStringField("lang", LanguageManager::GetLanguageKey());
	

	FString QueryStr = TUHelper::CombinParameters(query);
	FString UrlStr = cfgMd->configs.webPayUrl;
	auto Parse = TUCommon::FURL_RFC3986();
	Parse.Parse(UrlStr);
	UrlStr = FString::Printf(TEXT("%s://%s?%s"), *Parse.GetScheme(), *Parse.GetHost(), *QueryStr);
	return UrlStr;
}

void XDGImplement::RequestKidToken(TSharedPtr<FJsonObject> paras,
                                   TFunction<void(TSharedPtr<FTokenModel> kidToken)> resultBlock,
                                   TFunction<void(FXDGError error)> ErrorBlock) {
	XDGNet::RequestKidToken(paras, [=](TSharedPtr<FTokenModel> kidToken, FXDGError error) {
		if (error.code == Success && kidToken != nullptr) {
			kidToken->SaveToLocal();
			resultBlock(kidToken);
		}
		else {
			auto localToken = FTokenModel::GetLocalModel();
			if (localToken == nullptr) {
				ErrorBlock(error);
			}
			else {
				resultBlock(kidToken);
			}
		}
	});
}

void XDGImplement::RequestUserInfo(bool saveToLocal,
                                   TFunction<void(TSharedPtr<FXDGUser> model)> callback,
                                   TFunction<void(FXDGError error)> ErrorBlock) {
	XDGNet::RequestUserInfo(
		[=](TSharedPtr<FXDGUser> user, FXDGError error) {
			if (error.code == Success && user != nullptr) {
				if (saveToLocal) {
					user->SaveToLocal();
				}
				callback(user);
			}
			else {
				auto localUser = FXDGUser::GetLocalModel();
				if (localUser == nullptr) {
					ErrorBlock(error);
				}
				else {
					callback(localUser);
				}
			}
		});

}

void XDGImplement::AsyncNetworkTdsUser(const FString& userId,
                                       TFunction<void(FString SessionToken)> callback,
                                       TFunction<void(FXDGError error)> ErrorBlock) {
	XDGNet::RequestSyncToken(
		[=](TSharedPtr<FSyncTokenModel> model, FXDGError error) {
			if (error.code == Success && model != nullptr) {
				model->SaveToLocal();
				AsyncLocalTdsUser(userId, model->sessionToken);
				callback(model->sessionToken);
			}
			else {
				auto localModel = FSyncTokenModel::GetLocalModel();
				if (localModel.IsValid()) {
					AsyncLocalTdsUser(userId, localModel->sessionToken);
					callback(localModel->sessionToken);
				} else {
					ErrorBlock(error);
				}
			}
		}
	);
}

void XDGImplement::AsyncLocalTdsUser(const FString& userId, const FString& sessionToken) {
	// LCUser lcUser = LCObject.CreateWithoutData(LCUser.CLASS_NAME, userId) as LCUser;
	// lcUser.SessionToken = token;
	// await lcUser.SaveToLocal();
}

void XDGImplement::CheckPrivacyAlert(TFunction<void()> Callback) {
	if (FInitConfigModel::CanShowPrivacyAlert()) {
		UXDGPrivacyWidget::ShowPrivacy(
			[=](bool result) {
				if (result) {
					Callback();
				}
			});
	}
	else {
		Callback();
	}
}

void XDGImplement::RequestTapToken(TFunction<void(FTUAccessToken AccessToken)> callback,
                                   TFunction<void(FXDGError error)> ErrorBlock) {
	TapUELogin::Login(
		[=](TUAuthResult Result) {
			if (Result.GetType() == TUAuthResult::Success) {
				callback(*Result.GetToken().Get());
			}
			else if (Result.GetType() == TUAuthResult::Cancel) {
				FXDGError Error;
				Error.msg = "Login Cancel";
				Error.code = FTUError::ERROR_CODE_LOGIN_CANCEL;
				ErrorBlock(Error);
			}
			else if (Result.GetType() == TUAuthResult::Fail) {
				FXDGError Error;
				Error.msg = Result.GetError()->error_description;
				Error.code = Result.GetError()->code;
				ErrorBlock(Error);
			}
			else {
				ErrorBlock(FXDGError("Login Fail"));
			}
		});
}
