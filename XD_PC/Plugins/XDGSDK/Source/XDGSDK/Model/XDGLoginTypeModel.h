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
	

	static FString GetName(int Type) {
		if (Type == (int)LoginType::TapTap)
		{
			return  "TapTap";
		} else if (Type == (int)LoginType::Guest)
		{
			return  "Guest";
		}
		return "";
	}

	FXDGLoginTypeModel(LoginType type) : type(type), typeValue((int)type)
	{
		typeName = GetName((int)type);
	}
	
};
