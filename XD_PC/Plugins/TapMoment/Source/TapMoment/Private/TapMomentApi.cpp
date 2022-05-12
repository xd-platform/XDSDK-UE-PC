#include "TapMomentApi.h"

#include "TapConfig.h"

void TapMomentApi::Init(TapMomentConfig Config) {
	FTapConfig::Get().regionType = Config.IsCN ? RegionType::CN : RegionType::IO;
	FTapConfig::Get().MomentConfig.AppId = Config.AppId;
}

void TapMomentApi::OpenWebTopic() {
	FString Url = FTapConfig::Get().regionType == RegionType::CN ? "https://www.taptap.com" : "https://www.taptap.io";
	Url = Url / "app" / FTapConfig::Get().MomentConfig.AppId / "topic?utm_medium=link&utm_source=pc_sdk";
	FPlatformProcess::LaunchURL(*Url, nullptr, nullptr);
}
