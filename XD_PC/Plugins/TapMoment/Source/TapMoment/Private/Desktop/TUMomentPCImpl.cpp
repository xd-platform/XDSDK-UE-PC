#include "TUMomentPCImpl.h"

void TUMomentPCImpl::Init(const TUMomentType::Config& InitConfig) {
	Config = InitConfig;
}

void TUMomentPCImpl::OpenWebTopic() {
	FString Url;
	if (TUMomentImpl::Get()->Config.RegionType == TUType::CN) {
		Url = "https://www.taptap.com/app" / TUMomentImpl::Get()->Config.AppId_CN / "topic?utm_medium=link&utm_source=pc_sdk";
	} else {
		Url = "https://www.taptap.io/app" / TUMomentImpl::Get()->Config.AppId_IO + "?utm_medium=link&utm_source=pc_sdk";
	}
	FPlatformProcess::LaunchURL(*Url, nullptr, nullptr);
}
