#pragma once
#include "DataStorage.h"
#include "CoreMinimal.h"
#include "XDGStorage.generated.h"

USTRUCT()
struct FXDGStorage
{
	GENERATED_BODY()
	static FString InitConfig;
	static FString IpInfo;
	static FString TokenInfo;
	static FString UserInfo;
	static FString ClientId ;
	static FString PrivacyKey;
	static FString SessionTokenKey;
};




