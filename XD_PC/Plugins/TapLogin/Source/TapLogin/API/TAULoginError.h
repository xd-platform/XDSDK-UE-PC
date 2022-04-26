#pragma once
#include "CoreMinimal.h"
#include "TAULoginError.generated.h"

USTRUCT()
struct FTAULoginError
{
	GENERATED_BODY()
	UPROPERTY()
	int code;

	UPROPERTY()
	FString msg;

	UPROPERTY()
	FString error;

	UPROPERTY()
	FString error_description;
	
};
