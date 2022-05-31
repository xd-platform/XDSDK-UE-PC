#include "XUImpl.h"
#include "TapUEBootstrap.h"
#include "XUStorage.h"
#include "TUDeviceInfo.h"
#include "TUJsonHelper.h"
#include "XULanguageManager.h"
#include "TapUELogin.h"
#include "TUHelper.h"
#include "TUHUD.h"
#include "URLParser.h"
#include "XDGSDK.h"
#include "XDGSDK/UI/XUPrivacyWidget.h"

static int Success = 200;

void XUImpl::GetIpInfo(TFunction<void(TSharedPtr<FXUIpInfoModel> model, FString msg)> resultBlock) {
	XUNet::RequestIpInfo([=](TSharedPtr<FXUIpInfoModel> model, FXUError error) {
		if (model == nullptr) {
			TSharedPtr<FXUIpInfoModel> infoModel = FXUIpInfoModel::GetLocalModel();
			if (resultBlock) { resultBlock(infoModel, error.msg); }
		}
		else {
			model->SaveToLocal();
			if (resultBlock) { resultBlock(model, "success"); }
		}
	});
}

void XUImpl::InitSDK(FString sdkClientId, TFunction<void(bool successed, FString msg)> resultBlock) {
	TUDataStorage<FXUStorage>::SaveString(FXUStorage::ClientId, sdkClientId, false);
	XUNet::RequestConfig([=](TSharedPtr<FXUInitConfigModel> model, FXUError error) {
		if (model != nullptr && error.code == Success) {
			model->SaveToLocal();
			InitBootstrap(model, resultBlock, error.msg);
		}
		else {
			InitBootstrap(FXUInitConfigModel::GetLocalModel(), resultBlock, error.msg);
		}
	});
}

void XUImpl::InitBootstrap(const TSharedPtr<FXUInitConfigModel>& model,
                                 TFunction<void(bool successed, FString msg)> resultBlock, const FString& msg) {
	if (model == nullptr) {
		if (resultBlock) { resultBlock(false, msg); }
		return;
	}
	auto tapCfg = model->configs.tapSdkConfig;
	TUType::Config Config;
	Config.ClientID = tapCfg.clientId;
	Config.ClientToken = tapCfg.clientToken;
	Config.ServerURL = tapCfg.serverUrl;
	if (XUImpl::Get()->Config.RegionType == XUType::CN) {
		Config.RegionType = TUType::CN;
	} else {
		Config.RegionType = TUType::IO;
	}
	Config.DBConfig.Enable = tapCfg.enableTapDB;
	Config.DBConfig.Channel = tapCfg.tapDBChannel;
	Config.DBConfig.GameVersion = XUImpl::Get()->Config.GameVersion;
	TapUEBootstrap::Init(Config);

	if (resultBlock) { resultBlock(true, msg); }
}


void XUImpl::LoginByType(XUType::LoginType LoginType,
                               TFunction<void(TSharedPtr<FXUUser> user)> resultBlock,
                               TFunction<void(FXUError error)> ErrorBlock) {
	auto lmd = XULanguageManager::GetCurrentModel();
	if (LoginType == XUType::Default) {
		auto localUser = FXUUser::GetLocalModel();
		if (localUser.IsValid()) {
			RequestUserInfo(true, [=](TSharedPtr<FXUUser> user) {
				AsyncLocalTdsUser(user->userId, FXUSyncTokenModel::GetLocalModel()->sessionToken);
				resultBlock(user);
			}, ErrorBlock);
		}
		else {
			ErrorBlock(FXUError(lmd->tds_login_failed));
		}
	}
	else {
		GetLoginParam(LoginType, [=](TSharedPtr<FJsonObject> paras) {
			UTUHUD::ShowWait();
			TFunction<void(FXUError error)> ErrorCallBack = [=](FXUError error) {
				UTUHUD::Dismiss();
				if (ErrorBlock) {
					ErrorBlock(error);
				}
			};
			RequestKidToken(paras, [=](TSharedPtr<FXUTokenModel> kidToken) {
				RequestUserInfo(false, [=](TSharedPtr<FXUUser> user) {
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

void XUImpl::GetLoginParam(XUType::LoginType LoginType,
                                 TFunction<void(TSharedPtr<FJsonObject> paras)> resultBlock,
                                 TFunction<void(FXUError error)> ErrorBlock) {
	if (LoginType == XUType::Guest) {
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		JsonObject->SetNumberField("type", (int)LoginType);
		JsonObject->SetStringField("token", TUDeviceInfo::GetLoginId());
		resultBlock(JsonObject);
	}
	else if (LoginType == XUType::TapTap) {
		RequestTapToken(
			[=](FTUAccessToken AccessToken) {
				TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
				JsonObject->SetNumberField("type", (int)LoginType);
				JsonObject->SetStringField("token", AccessToken.kid);
				JsonObject->SetStringField("secret", AccessToken.mac_key);
				resultBlock(JsonObject);
			}, ErrorBlock);
	}
	else {
		ErrorBlock(FXUError("No Login Param"));
	}
}

void XUImpl::CheckPay(TFunction<void(XUType::CheckPayType CheckType)> SuccessBlock,
                            TFunction<void(const FXUError& Error)> FailBlock) {
	XUNet::CheckPay([=](TSharedPtr<FXUPayCheckResponseModel> Model, FXUError Error) {
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
					SuccessBlock(XUType::iOSAndAndroid);
				}
				else if (hasIOS) {
					SuccessBlock(XUType::iOS);
				}
				else if (hasAndroid) {
					SuccessBlock(XUType::Android);
				}
				else {
					SuccessBlock(XUType::None);
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

FString XUImpl::GetCustomerCenter(const FString& ServerId, const FString& RoleId, const FString& RoleName) {
	auto userMd = FXUUser::GetLocalModel();
	auto cfgMd = FXUInitConfigModel::GetLocalModel();
	auto tkModel = FXUTokenModel::GetLocalModel();
	if (!userMd.IsValid() || !cfgMd.IsValid() || !tkModel.IsValid()) {
		return "";
	}
	
	TSharedPtr<FJsonObject> query = MakeShareable(new FJsonObject);
	query->SetStringField("client_id", TUDataStorage<FXUStorage>::LoadString(FXUStorage::ClientId));
	query->SetStringField("access_token", tkModel->kid);
	query->SetStringField("user_id", userMd->userId);
	query->SetStringField("server_id", ServerId);
	query->SetStringField("role_id", RoleId);
	query->SetStringField("role_name", RoleName);
	query->SetStringField("region", cfgMd->configs.region);
	query->SetStringField("sdk_ver", XDUESDK_VERSION);
	query->SetStringField("sdk_lang", XULanguageManager::GetCustomerCenterLang());
	query->SetStringField("app_ver", XUImpl::Get()->Config.GameVersion);
	query->SetStringField("app_ver_code", XUImpl::Get()->Config.GameVersion);
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

FString XUImpl::GetPayUrl(const FString& ServerId, const FString& RoleId) {
	auto userMd = FXUUser::GetLocalModel();
	auto cfgMd = FXUInitConfigModel::GetLocalModel();
	if (!userMd.IsValid() || !cfgMd.IsValid()) {
		return "";
	}
	
	TSharedPtr<FJsonObject> query = MakeShareable(new FJsonObject);

	query->SetStringField("serverId", ServerId);
	query->SetStringField("roleId", RoleId);
	query->SetStringField("region", cfgMd->configs.region);
	query->SetStringField("appId", cfgMd->configs.appId);
	query->SetStringField("lang", XULanguageManager::GetLanguageKey());
	

	FString QueryStr = TUHelper::CombinParameters(query);
	FString UrlStr = cfgMd->configs.webPayUrl;
	auto Parse = TUCommon::FURL_RFC3986();
	Parse.Parse(UrlStr);
	UrlStr = FString::Printf(TEXT("%s://%s?%s"), *Parse.GetScheme(), *Parse.GetHost(), *QueryStr);
	return UrlStr;
}

TSharedPtr<XUImpl> XUImpl::Instance = nullptr;

TSharedPtr<XUImpl>& XUImpl::Get() {
	if (!Instance.IsValid()) {
		Instance = MakeShareable(new XUImpl);
	}
	return Instance;
}

void XUImpl::RequestKidToken(TSharedPtr<FJsonObject> paras,
                             TFunction<void(TSharedPtr<FXUTokenModel> kidToken)> resultBlock,
                             TFunction<void(FXUError error)> ErrorBlock) {
	XUNet::RequestKidToken(paras, [=](TSharedPtr<FXUTokenModel> kidToken, FXUError error) {
		if (error.code == Success && kidToken != nullptr) {
			kidToken->SaveToLocal();
			resultBlock(kidToken);
		}
		else {
			auto localToken = FXUTokenModel::GetLocalModel();
			if (localToken == nullptr) {
				ErrorBlock(error);
			}
			else {
				resultBlock(kidToken);
			}
		}
	});
}

void XUImpl::RequestUserInfo(bool saveToLocal,
                                   TFunction<void(TSharedPtr<FXUUser> model)> callback,
                                   TFunction<void(FXUError error)> ErrorBlock) {
	XUNet::RequestUserInfo(
		[=](TSharedPtr<FXUUser> user, FXUError error) {
			if (error.code == Success && user != nullptr) {
				if (saveToLocal) {
					user->SaveToLocal();
				}
				callback(user);
			}
			else {
				auto localUser = FXUUser::GetLocalModel();
				if (localUser == nullptr) {
					ErrorBlock(error);
				}
				else {
					callback(localUser);
				}
			}
		});

}

void XUImpl::AsyncNetworkTdsUser(const FString& userId,
                                       TFunction<void(FString SessionToken)> callback,
                                       TFunction<void(FXUError error)> ErrorBlock) {
	XUNet::RequestSyncToken(
		[=](TSharedPtr<FXUSyncTokenModel> model, FXUError error) {
			if (error.code == Success && model != nullptr) {
				model->SaveToLocal();
				AsyncLocalTdsUser(userId, model->sessionToken);
				callback(model->sessionToken);
			}
			else {
				auto localModel = FXUSyncTokenModel::GetLocalModel();
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

void XUImpl::AsyncLocalTdsUser(const FString& userId, const FString& sessionToken) {
	// LCUser lcUser = LCObject.CreateWithoutData(LCUser.CLASS_NAME, userId) as LCUser;
	// lcUser.SessionToken = token;
	// await lcUser.SaveToLocal();
}

void XUImpl::CheckPrivacyAlert(TFunction<void()> Callback) {
	if (FXUInitConfigModel::CanShowPrivacyAlert() && XUImpl::Get()->Config.RegionType == XUType::IO) {
		UXUPrivacyWidget::ShowPrivacy(
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

void XUImpl::RequestTapToken(TFunction<void(FTUAccessToken AccessToken)> callback,
                                   TFunction<void(FXUError error)> ErrorBlock) {
	TapUELogin::Login(
		[=](TUAuthResult Result) {
			if (Result.GetType() == TUAuthResult::Success) {
				callback(*Result.GetToken().Get());
			}
			else if (Result.GetType() == TUAuthResult::Cancel) {
				FXUError Error;
				Error.msg = "Login Cancel";
				Error.code = FTUError::ERROR_CODE_LOGIN_CANCEL;
				ErrorBlock(Error);
			}
			else if (Result.GetType() == TUAuthResult::Fail) {
				FXUError Error;
				Error.msg = Result.GetError()->error_description;
				Error.code = Result.GetError()->code;
				ErrorBlock(Error);
			}
			else {
				ErrorBlock(FXUError("Login Fail"));
			}
		});
}
