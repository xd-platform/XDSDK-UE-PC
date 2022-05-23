#pragma once
#include "CoreMinimal.h"
#include "TUAccessToken.generated.h"

USTRUCT(BlueprintType)
struct FTUAccessToken
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
	uint64 scope;

	void SaveToLocal() const;
	
	static TSharedPtr<FTUAccessToken> GetLocalModel();

	static void ClearLocalModel();
	
};
