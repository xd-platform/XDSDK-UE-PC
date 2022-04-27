#pragma once
#include "CoreMinimal.h"
#include "TAULoginDataStorageName.h"
#include "TapAccessToken.generated.h"

USTRUCT(BlueprintType)
struct FTapAccessToken
{
	GENERATED_BODY()

	UPROPERTY()
	FString kid;

	UPROPERTY()
	FString access_token;

	UPROPERTY()
	FString token_type;

	UPROPERTY()
	FString mac_key;

	UPROPERTY()
	FString mac_algorithm;

	UPROPERTY()
	uint64 expireIn;

	void SaveToLocal()
	{
		DataStorage::SaveStruct(TAULoginDataStorageName::AccessToken, *this, true);
	}
	
	static TSharedPtr<FTapAccessToken> GetLocalModel()
	{
		return  DataStorage::LoadStruct<FTapAccessToken>(TAULoginDataStorageName::AccessToken);
	}
	
};
