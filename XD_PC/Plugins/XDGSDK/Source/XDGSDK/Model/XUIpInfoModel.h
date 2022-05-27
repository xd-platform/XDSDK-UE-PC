#pragma once
#include "CoreMinimal.h"
#include "XUStorage.h"
#include "XUIpInfoModel.generated.h"

USTRUCT()
struct FXUIpInfoModel
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
		TUDataStorage<FXUStorage>::SaveStruct(FXUStorage::IpInfo, *this, true);
	}

	static TSharedPtr<FXUIpInfoModel> GetLocalModel()
	{
		return  TUDataStorage<FXUStorage>::LoadStruct<FXUIpInfoModel>(FXUStorage::IpInfo);
	}
};
