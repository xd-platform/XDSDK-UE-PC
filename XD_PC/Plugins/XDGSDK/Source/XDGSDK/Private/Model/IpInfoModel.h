#pragma once

// {"country":"Hong Kong","city":"","latitude":"22.2578","longitude":"114.1657","timeZone":"Asia/Hong_Kong","country_code":"HK","continent_code":"AS","src_ip":"103.86.76.195"} 
class IpInfoModel
{
public:
	FString city;
	FString country;
	FString country_code;
	FString latitude;
	FString longitude;
	FString src_ip;
	FString timeZone;
};
