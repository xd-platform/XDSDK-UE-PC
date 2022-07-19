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
#include "XUConfigManager.h"
#include "XULoginHelper.h"
#include "XDGSDK/UI/XUPayWebWidget.h"
#include "XDGSDK/UI/XUPrivacyWidget.h"

static int Success = 200;

void XUImpl::InitSDK(const FString& GameVersion, XUInitCallback CallBack) {
	XUConfigManager::ReadLocalConfig([=](TSharedPtr<XUType::Config> Config, const FString& Msg) {
		if (Config.IsValid()) {
			Config->GameVersion = GameVersion;
			Config->TapConfig.DBConfig.GameVersion = GameVersion;
			InitSDK(Config, CallBack);
		} else {
			if (CallBack) {
				CallBack(false, Msg);
			}
		}
	});
}

void XUImpl::InitSDK(TSharedPtr<XUType::Config> Config, XUInitCallback CallBack) {
	XUConfigManager::LoadRemoteOrCachedServiceTerms(Config, [=](TSharedPtr<XUType::Config> ConfigTerms, const FString& Msg) {
		if (ConfigTerms.IsValid()) {
			XUConfigManager::SetConfig(ConfigTerms);
			CheckAgreement(ConfigTerms, CallBack);
		} else {
			if (CallBack) {
				CallBack(false, Msg);
			}
		}
	});
}

void XUImpl::LoginByType(XUType::LoginType LoginType,
                         TFunction<void(TSharedPtr<FXUUser> user)> resultBlock,
                         TFunction<void(FXUError error)> ErrorBlock) {
	auto lmd = XULanguageManager::GetCurrentModel();
	if (LoginType == XUType::Default) {
		auto localUser = FXUUser::GetLocalModel();
		if (localUser.IsValid()) {
			RequestUserInfo(true, [](TSharedPtr<FXUUser> user) {}, [](FXUError Error) {});
			AsyncLocalTdsUser(localUser->userId, FXUSyncTokenModel::GetLocalModel()->sessionToken);
			resultBlock(localUser);
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
						user->SaveToLocal();
						resultBlock(user);
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
		XULoginHelper::TapTapLogin(
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
	auto cfgMd = XUConfigManager::CurrentConfig();
	auto tkModel = FXUTokenModel::GetLocalModel();
	if (!userMd.IsValid() || !cfgMd.IsValid() || !tkModel.IsValid()) {
		return "";
	}
	
	TSharedPtr<FJsonObject> query = MakeShareable(new FJsonObject);
	query->SetStringField("client_id", XUConfigManager::CurrentConfig()->ClientId);
	query->SetStringField("access_token", tkModel->kid);
	query->SetStringField("user_id", userMd->userId);
	query->SetStringField("server_id", ServerId);
	query->SetStringField("role_id", RoleId);
	query->SetStringField("role_name", RoleName);
	query->SetStringField("region", cfgMd->Region);
	query->SetStringField("sdk_ver", XDUESDK_VERSION);
	query->SetStringField("sdk_lang", XULanguageManager::GetCustomerCenterLang());
	query->SetStringField("app_ver", XUConfigManager::CurrentConfig()->GameVersion);
	query->SetStringField("app_ver_code", XUConfigManager::CurrentConfig()->GameVersion);
	query->SetStringField("res", FString::Printf(TEXT("%d_%d"), TUDeviceInfo::GetScreenWidth(), TUDeviceInfo::GetScreenHeight()));
	query->SetStringField("cpu", TUDeviceInfo::GetCPU());
	query->SetStringField("pt", TUDeviceInfo::GetPlatform());
	query->SetStringField("os", TUDeviceInfo::GetOSVersion());
	query->SetStringField("brand", TUDeviceInfo::GetGPU());
	query->SetStringField("game_name", TUDeviceInfo::GetProjectName());

	FString QueryStr = TUHelper::CombinParameters(query);
	FString UrlStr = cfgMd->ReportUrl;
	auto Parse = TUCommon::FURL_RFC3986();
	Parse.Parse(UrlStr);
	UrlStr = FString::Printf(TEXT("%s://%s"), *Parse.GetScheme(), *Parse.GetHost()) / Parse.GetPath();
	UrlStr = UrlStr + "?" + QueryStr;
	return UrlStr;
}

FString XUImpl::GetPayUrl(const FString& ServerId, const FString& RoleId) {
	auto userMd = FXUUser::GetLocalModel();
	auto cfgMd = XUConfigManager::CurrentConfig();
	if (!userMd.IsValid() || !cfgMd.IsValid()) {
		return "";
	}
	
	TSharedPtr<FJsonObject> query = MakeShareable(new FJsonObject);

	query->SetStringField("serverId", ServerId);
	query->SetStringField("roleId", RoleId);
	query->SetStringField("region", cfgMd->Region);
	query->SetStringField("appId", cfgMd->AppID);
	query->SetStringField("lang", XULanguageManager::GetLanguageKey());
	

	FString QueryStr = TUHelper::CombinParameters(query);
	FString UrlStr = cfgMd->WebPayUrl;
	auto Parse = TUCommon::FURL_RFC3986();
	Parse.Parse(UrlStr);
	UrlStr = FString::Printf(TEXT("%s://%s"), *Parse.GetScheme(), *Parse.GetHost()) / Parse.GetPath();
	UrlStr = UrlStr + "?" + QueryStr;
	return UrlStr;
}

void XUImpl::OpenWebPay(const FString& ServerId, const FString& RoleId, const FString& ProductSkuCode,
	const FString& ProductName, float PayAmount, TFunction<void(XUType::PayResult Result)> CallBack,
	const FString& Ext) {
	
	TSharedPtr<FJsonObject> query = MakeShareable(new FJsonObject);

	query->SetStringField("serverId", ServerId);
	query->SetStringField("roleId", RoleId);
	query->SetStringField("productSkuCode", ProductSkuCode);
	query->SetStringField("region", XUConfigManager::CurrentConfig()->Region);
	query->SetStringField("appId", XUConfigManager::CurrentConfig()->AppID);
	query->SetStringField("lang", XULanguageManager::GetLanguageKey());
	query->SetStringField("platform", "pc");

	if (!ProductName.IsEmpty()) {
		query->SetStringField("productName", ProductName);
	}
	if (PayAmount > 0) {
		query->SetStringField("payAmount", FString::Printf(TEXT("%f"), PayAmount));
	}
	if (!Ext.IsEmpty()) {
		query->SetStringField("extras", Ext);
	}
	// PayWebBrowser->LoadURL("https://sdkpay-test.xd.cn/qr-pay/?productSkuCode=com.xd.sdkdemo1.stone30&payAmount=30.0&orderId=&roleId=323499800362549248&appId=1010&product_id%7Ccom.xd.sdkdemo1.stone30&region=CN&lang=zh_CN&serverId=demo_server_id&productName=com.xd.sdkdemo1.stone30");

	FString QueryStr = TUHelper::CombinParameters(query);
	FString UrlStr = XUConfigManager::CurrentConfig()->WebPayUrl;
	auto Parse = TUCommon::FURL_RFC3986();
	Parse.Parse(UrlStr);
	UrlStr = FString::Printf(TEXT("%s://%s"), *Parse.GetScheme(), *Parse.GetHost()) / Parse.GetPath();
	UrlStr = UrlStr + "?" + QueryStr;

	UXUPayWebWidget::Show(UrlStr, CallBack);
}


void XUImpl::ResetPrivacy() {
	TUDataStorage<FXUStorage>::Remove(FXUStorage::PrivacyKey);
	TUDataStorage<FXUStorage>::Remove(XUConfigManager::GetRegionAgreementCacheName());
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
		}, [=]() {
			if (saveToLocal) {
				FXUUser::ClearUserData();
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

void XUImpl::CheckAgreement(TSharedPtr<XUType::Config> Config, XUInitCallback CallBack) {
	if (!XUConfigManager::NeedShowAgreement()) {
		InitFinish(CallBack);
		return;
	}
	UXUPrivacyWidget::ShowPrivacy([=]() {
		InitFinish(CallBack);
	});
}

void XUImpl::InitFinish(XUInitCallback CallBack) {
	XUConfigManager::InitTapSDK();
	XUConfigManager::SetGameInited();
	if (CallBack) {
		CallBack(true, "");
	}
	RequestServerConfig();
	XUConfigManager::UploadUserAgreement();
}

void XUImpl::RequestServerConfig() {
	XUConfigManager::RequestServerConfig(false);
	XUConfigManager::GetRegionInfo([](TSharedPtr<FXUIpInfoModel> ModelPtr) {
		
	});
}
