#pragma once
#include "CoreMinimal.h"
#include "TapConfig.generated.h"

UENUM()
enum class RegionType: int8
{
	CN = 0,
	IO = 1
};


USTRUCT()
struct TAPCOMMON_API FTapDBConfig
{
	GENERATED_BODY()

	UPROPERTY()
	bool enable;
	
	UPROPERTY()
	FString channel;

	UPROPERTY()
	FString gameVersion;

	UPROPERTY()
	bool advertiserIDCollectionEnabled;
	
};

USTRUCT()
struct TAPCOMMON_API FTapMomentConfig
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString AppId;
	
};

USTRUCT()
struct TAPCOMMON_API FTapConfig
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString clientID;

	UPROPERTY()
	FString clientToken;

	UPROPERTY()
	RegionType regionType;

	UPROPERTY()
	FString serverURL;

	UPROPERTY()
	FTapDBConfig dbConfig;

	UPROPERTY()
	FTapMomentConfig MomentConfig;

	static FTapConfig& Get();

private:
	static FTapConfig * Singleton;
	
};



