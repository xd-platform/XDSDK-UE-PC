#pragma once

class TUDBRegionConfig
{
public:
	virtual ~TUDBRegionConfig() = default;
	virtual FString GetHost() = 0;

	FString GetEventUrl();
	FString GetCustomUrl();
	FString GetIdentifyUrl();
	
	static TSharedPtr<TUDBRegionConfig>& Get();

private:
	static TSharedPtr<TUDBRegionConfig> Instance;
	
};
