#include "TapTapSdk.h"
#include "TapLocalizeManager.h"

FString TapTapSdk::Version = "1.2.0";
FString TapTapSdk::ClientId = "";
TSharedPtr<Region> TapTapSdk::CurrentRegion = nullptr;

void TapTapSdk::SDKInitialize(const FString& clientId, bool isCn)
{
	TapTapSdk::ClientId = clientId;
	TapTapSdk::CurrentRegion = isCn ? MakeShareable((Region *)new RegionCN) : MakeShareable((Region *)new RegionIO);
	TapLocalizeManager::SetCurrentRegion(isCn);
	// Region
}



