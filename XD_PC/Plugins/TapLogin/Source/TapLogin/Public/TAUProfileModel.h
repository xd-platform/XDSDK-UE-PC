#pragma once
#include "CoreMinimal.h"
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

	void SaveToLocal();
	
	static TSharedPtr<FTAUProfileModel> GetLocalModel();

	static void ClearLocalModel();
	
};
