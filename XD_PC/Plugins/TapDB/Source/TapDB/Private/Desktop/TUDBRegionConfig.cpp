#include "TUDBRegionConfig.h"

#include "TUDBImpl.h"

class TUDBRegionConfigCN: public TUDBRegionConfig
{
public:
	virtual FString GetHost() {
		return "https://e.tapdb.net/";
	}
};

class TUDBRegionConfigIO: public TUDBRegionConfig
{
public:
	virtual FString GetHost() {
		return "https://e.tapdb.ap-sg.tapapis.com/";
	}
};

TSharedPtr<TUDBRegionConfig> TUDBRegionConfig::Instance = nullptr;


FString TUDBRegionConfig::GetEventUrl() {
	return GetHost() / "event";
}

FString TUDBRegionConfig::GetCustomUrl() {
	return GetHost() / "custom";
}

FString TUDBRegionConfig::GetIdentifyUrl() {
	return GetHost() / "identify";
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


