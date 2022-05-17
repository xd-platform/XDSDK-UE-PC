#include "TapMomentApi.h"

#include "TUCommonConfig.h"

void TapMomentApi::Init(TapMomentConfig Config) {
	FTUCommonConfig::Get().regionType = Config.IsCN ? RegionType::CN : RegionType::IO;
	FTUCommonConfig::Get().MomentConfig.AppId = Config.AppId;
}

void TapMomentApi::OpenWebTopic() {
	FString Url = FTUCommonConfig::Get().regionType == RegionType::CN ? "https://www.taptap.com" : "https://www.taptap.io";
	Url = Url / "app" / FTUCommonConfig::Get().MomentConfig.AppId / "topic?utm_medium=link&utm_source=pc_sdk";
	FPlatformProcess::LaunchURL(*Url, nullptr, nullptr);
}
