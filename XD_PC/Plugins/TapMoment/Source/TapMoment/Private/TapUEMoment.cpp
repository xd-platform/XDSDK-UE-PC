#include "TapUEMoment.h"
#include "TUMomentImpl.h"

void TapUEMoment::Init(TUMomentType::Config Config) {
	TUMomentImpl::Get()->Init(Config);
}

void TapUEMoment::OpenWebTopic() {
	FString Url = TUMomentImpl::Get()->Config.RegionType == TUType::CN ? "https://www.taptap.com" : "https://www.taptap.io";
	Url = Url / "app" /  TUType::Config::Get()->MomentConfig.AppID / "topic?utm_medium=link&utm_source=pc_sdk";
	FPlatformProcess::LaunchURL(*Url, nullptr, nullptr);
}
