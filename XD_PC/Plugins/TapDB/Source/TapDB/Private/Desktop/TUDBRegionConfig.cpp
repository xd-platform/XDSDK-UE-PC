#include "TUDBRegionConfig.h"

#include "TUDBImpl.h"

class TUDBRegionConfigCN: public TUDBRegionConfig
{
public:
	virtual FString GetHost() {
		return "https://e.tapdb.net/v2";
	}
};

class TUDBRegionConfigIO: public TUDBRegionConfig
{
public:
	virtual FString GetHost() {
		return "https://e.tapdb.ap-sg.tapapis.com/v2";
	}
};

TSharedPtr<TUDBRegionConfig> TUDBRegionConfig::Instance = nullptr;


FString TUDBRegionConfig::GetEventUrl() {
	return GetHost() / "event";
}

FString TUDBRegionConfig::GetCustomUrl() {
	return GetHost() / "event";
}

FString TUDBRegionConfig::GetIdentifyUrl() {
	return GetHost() / "event";
}

TSharedPtr<TUDBRegionConfig>& TUDBRegionConfig::Get() {
	if (!Instance.IsValid()) {
		if (TUDBImpl::Get()->Config.RegionType == TUType::CN) {
			Instance = MakeShareable(new TUDBRegionConfigCN);
		} else {
			Instance = MakeShareable(new TUDBRegionConfigIO);
		}
	}
	return Instance;
}


