#pragma once
#include "CoreMinimal.h"
#include "TAULoginStorage.generated.h"

USTRUCT()
struct FTAULoginStorage
{
	GENERATED_BODY()
	static FString AccessToken;
	static FString Profile;
};

