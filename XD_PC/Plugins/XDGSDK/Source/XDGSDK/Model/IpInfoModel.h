#pragma once
#include "CoreMinimal.h"
#include "XDGStorage.h"
#include "IpInfoModel.generated.h"

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

	void SaveToLocal()
	{
		DataStorage<FXDGStorage>::SaveStruct(FXDGStorage::IpInfo, *this, true);
	}

	static TSharedPtr<FIpInfoModel> GetLocalModel()
	{
		return  DataStorage<FXDGStorage>::LoadStruct<FIpInfoModel>(FXDGStorage::IpInfo);
	}
};
