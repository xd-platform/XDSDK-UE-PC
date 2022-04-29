#pragma once
#include "XDGEnumType.h"
#include "XDGLoginTypeModel.generated.h"


USTRUCT()
struct FXDGLoginTypeModel
{
	GENERATED_BODY()
	
	UPROPERTY()
	LoginType type;

	UPROPERTY()
	FString typeName;

	UPROPERTY()
	int typeValue;

	FXDGLoginTypeModel() = default;

	FXDGLoginTypeModel(LoginType type) : type(type), typeValue((int)type)
	{
		if (type == LoginType::TapTap)
		{
			typeName = "TapTap";
		} else if (type == LoginType::Guest)
		{
			typeName = "Guest";
		}
	}
	
};
