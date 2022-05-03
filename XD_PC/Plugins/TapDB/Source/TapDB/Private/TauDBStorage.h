#pragma once
#include "DataStorage.h"
#include "CoreMinimal.h"
#include "TauDBStorage.generated.h"

USTRUCT()
struct FTauDBStorage
{
	GENERATED_BODY()
	
	static FString ClientIdentifyKey;
	static FString TapDBPlayTime;
	
};
