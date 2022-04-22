#pragma once
#include "Region.h"

class TAPLOGIN_API TapTapSdk
{
public:
	static FString Version;
	static FString ClientId;
	static TSharedPtr<Region> CurrentRegion;

	static void SDKInitialize(const FString& clientId, bool isCn);
	
};
