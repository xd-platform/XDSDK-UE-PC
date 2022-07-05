#include "XUConfigManager.h"

#include "TUDataStorage.h"
#include "XULocalInitConfigModel.h"
#include "XUStorage.h"

XUConfigManager * XUConfigManager::Instance = nullptr;

XUConfigManager& XUConfigManager::SharedInstance() {
	if (Instance == nullptr) {
		Instance = new XUConfigManager();
		Instance->ConfigFileName = "XDConfig";
		Instance->Config = MakeShareable(new XUType::Config);
		Instance->AgreementUIEnable = true;
	}
	return *Instance;
}

TSharedPtr<XUType::Config> XUConfigManager::CurrentConfig() {
	return SharedInstance().Config;
}

void XUConfigManager::SetConfig(TSharedPtr<XUType::Config> Config) {
	SharedInstance().Config = Config;
	// UpdateHttpConfig();
}

bool XUConfigManager::IsCN() {
	return SharedInstance().Config->RegionType == XUType::CN;
}

void XUConfigManager::ReadLocalConfig(XUConfigHandler Handler) {
	auto Ptr = FXULocalInitConfig::GetLocalModel();
	if (!Ptr.IsValid()) {
		if (Handler) {
			Handler(false, nullptr, "Config file empty, Please check your XDConfig.json");
		}
		return;
	}
	auto InitPtr = Ptr->GenerateXUConfig();
	if (Handler) {
		Handler(true, InitPtr, "");
	}
}

void XUConfigManager::LoadRemoteOrCachedServiceTerms(TSharedPtr<XUType::Config> Config, XUConfigHandler Handler) {
	// 统一先用cache刷新一下
	
	// [self updateConfigWithDic:[self sharedInstance].localCache];
}

void XUConfigManager::RequestServerConfig(bool FirstRequest) {
}

void XUConfigManager::RequestServerConfig(bool FirstRequest, TFunction<void(bool Success)> Handler) {
}

void XUConfigManager::InitTapSDK() {
}

bool XUConfigManager::NeedShowAgreement() {
	return true;
}

FString XUConfigManager::GetAgreementUrl() {
	return "true";
}

void XUConfigManager::UploadUserAgreement() {
}

// void XUConfigManager::UpdateHttpConfig() {
// }

void XUConfigManager::SetGameInited() {
	SharedInstance().IsInited = true;
}

bool XUConfigManager::IsGameInited() {
	return SharedInstance().IsInited;
}

bool XUConfigManager::GoogleEnable() {
	if (SharedInstance().Config.IsValid() && !SharedInstance().Config->GoogleInfo.ClientID.IsEmpty()) {
		return true;
	}
	return false;
}

bool XUConfigManager::FacebookEnable() {
	if (SharedInstance().Config.IsValid() && !SharedInstance().Config->FacebookInfo.AppID.IsEmpty()) {
		return true;
	}
	return false;
}

bool XUConfigManager::TapTapEnable() {
	if (SharedInstance().Config.IsValid() && !SharedInstance().Config->TapConfig.ClientID.IsEmpty()) {
		return true;
	}
	return false;
}

bool XUConfigManager::TapDBEnable() {
	if (TapTapEnable() && SharedInstance().Config->TapConfig.DBConfig.Enable) {
		return true;
	}
	return false;
}

bool XUConfigManager::AdjustEnable() {
	if (SharedInstance().Config.IsValid() && !SharedInstance().Config->AdjustInfo.AppToken.IsEmpty()) {
		return true;
	}
	return false;
}

bool XUConfigManager::AppsflyersEnable() {
	if (SharedInstance().Config.IsValid() && !SharedInstance().Config->AppsflyerInfo.AppID.IsEmpty()) {
		return true;
	}
	return false;
}

bool XUConfigManager::LineEnable() {
	if (SharedInstance().Config.IsValid() && !SharedInstance().Config->LineInfo.ChannelID.IsEmpty()) {
		return true;
	}
	return false;
}

bool XUConfigManager::TwitterEnable() {
	if (SharedInstance().Config.IsValid()) {
		if (!SharedInstance().Config->TwitterInfo.ConsumerKey.IsEmpty() && !SharedInstance().Config->TwitterInfo.ConsumerSecret.IsEmpty()) {
			return true;
		}
	}
	return false;
}

bool XUConfigManager::NeedReportService() {
	if (SharedInstance().Config.IsValid() && !SharedInstance().Config->ReportUrl.IsEmpty()) {
		return true;
	}
	return false;
}

bool XUConfigManager::IsGameInKoreaAndPushServiceEnable() {
	bool IsKR = SharedInstance().Config->Agreement.Region == "kr";
	bool CanPush = SharedInstance().Config->Agreement.IsKRPushServiceSwitchEnable;
	if (IsKR && CanPush) {
		return true;
	}
	return false;
}

bool XUConfigManager::IsGameInNA() {
	return SharedInstance().Config->Agreement.Region == "us";
}

void XUConfigManager::UpdateConfig(TSharedPtr<XUType::Config> Config) {
	SetConfig(Config);
}

void XUConfigManager::RecordKRPushSetting(bool PushOn) {
	TUDataStorage<FXUStorage>::SaveBool(FXUStorage::AgreementKRPush, PushOn);
}

bool XUConfigManager::GetKRPushSetting() {
	if (IsGameInKoreaAndPushServiceEnable() && TUDataStorage<FXUStorage>::LoadBool(FXUStorage::AgreementKRPush)) {
		return true;
	}
	return false;
}

TSharedPtr<FJsonObject> XUConfigManager::GetLocalCache() {
	return TUDataStorage<FXUStorage>::LoadJsonObject(FXUStorage::XD_CACHE_CONFIG);
}
