#pragma once
#include "CoreMinimal.h"
#include "IpInfoModel.generated.h"

// {"country":"Hong Kong","city":"","latitude":"22.2578","longitude":"114.1657","timeZone":"Asia/Hong_Kong","country_code":"HK","continent_code":"AS","src_ip":"103.86.76.195"} 
USTRUCT()
struct FIpInfoModel
{
	GENERATED_BODY()

	UPROPERTY()
	FString city;
	
	UPROPERTY()
	FString country;

	UPROPERTY()
	FString country_code;

	UPROPERTY()
	FString latitude;

	UPROPERTY()
	FString longitude;

	UPROPERTY()
	FString src_ip;

	UPROPERTY()
	FString timeZone;
};
