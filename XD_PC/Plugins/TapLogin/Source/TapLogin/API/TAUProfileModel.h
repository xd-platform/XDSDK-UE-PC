#pragma once
#include "CoreMinimal.h"
#include "TAULoginDataStorageName.h"
#include "TAUProfileModel.generated.h"

USTRUCT()
struct FTAUProfileModel
{
	GENERATED_BODY()

	UPROPERTY()
	FString name;
	
	UPROPERTY()
	FString avatar;
	
	UPROPERTY()
	FString openid;
	
	UPROPERTY()
	FString unionid;

	void SaveToLocal()
	{
		DataStorage::SaveStruct(TAULoginDataStorageName::Profile, *this, true);
	}
	
	static TSharedPtr<FTAUProfileModel> GetLocalModel()
	{
		return  DataStorage::LoadStruct<FTAUProfileModel>(TAULoginDataStorageName::Profile);
	}
	
};
