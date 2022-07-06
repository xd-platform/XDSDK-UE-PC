#pragma once
#include "XUServerConfig.h"
#include "XUType.h"

typedef TFunction<void(bool Success, TSharedPtr<XUType::Config> Config, const FString& Msg)> XUConfigHandler;

class XUConfigManager {
public:
	FString TargetRegion;
	bool AgreementUIEnable;
	FString ConfigFileName;

	static XUConfigManager& SharedInstance();
	
	static TSharedPtr<XUType::Config> CurrentConfig();

	static void SetConfig(TSharedPtr<XUType::Config> Config);

	static bool IsCN();

	static void ReadLocalConfig(XUConfigHandler Handler);

	static void LoadRemoteOrCachedServiceTerms(TSharedPtr<XUType::Config> Config, XUConfigHandler Handler);

	static void RequestServerConfig(bool FirstRequest);

	static void RequestServerConfig(bool FirstRequest, TFunction<void(bool Success)> Handler);

	static void InitTapSDK();

	static bool NeedShowAgreement();

	static FString GetAgreementUrl();

	static void UploadUserAgreement();

	// static void UpdateBindEntriesConfig:(NSArray *)config;

	// static void UpdateHttpConfig();

	// + (XDGRegionInfo *)GetRegionInfo();

	// static void GetRegionInfo:(void (^)(XDGRegionInfo *result))completeHandler;

	static void SetGameInited();
	static bool IsGameInited();

	static bool GoogleEnable();
	static bool FacebookEnable();
	static bool TapTapEnable();
	static bool TapDBEnable();
	static bool AdjustEnable();
	static bool AppsflyersEnable();
	static bool LineEnable();
	static bool TwitterEnable();

	static bool NeedReportService();
	static bool IsGameInKoreaAndPushServiceEnable();
	static bool IsGameInNA();


	static void UpdateConfig(TSharedPtr<XUType::Config> Config);

	static void RecordKRPushSetting(bool PushOn);

	static bool GetKRPushSetting();
	
private:
	
	static XUConfigManager * Instance;
	TSharedPtr<FJsonObject> GetLocalCache();
	bool IsInited = false;
	TSharedPtr<XUType::Config> Config;
	bool ConfigRequestSuccess = false;

	static FString GetRegionAgreementCacheName();

	static XUType::AgreementConfig GenerateAgreementConfig(const TSharedPtr<FJsonObject>& JsonObject);
	static void SaveAgreementConfig(XUType::AgreementConfig& AgreementConfig, bool Upload);

	void UpdateConfig(TSharedPtr<FXUServerConfig> ServerConfig);
};