#pragma once
#include "CoreMinimal.h"
#include "TAUQrCodeModel.generated.h"

USTRUCT()
struct FTAUQrCodeModel
{
	GENERATED_BODY()

	UPROPERTY()
	FString qrcode_url;

	UPROPERTY()
	FString verification_url;

	UPROPERTY()
	FString device_code;

	UPROPERTY()
	FString user_code;

	UPROPERTY()
	int64 expires_in;

	UPROPERTY()
	int64 interval;
	
};
