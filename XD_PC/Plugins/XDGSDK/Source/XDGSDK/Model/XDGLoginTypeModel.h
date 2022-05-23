#pragma once
#include "XDEnumType.h"
#include "XDGLoginTypeModel.generated.h"


USTRUCT()
struct FXDGLoginTypeModel
{
	GENERATED_BODY()
	
	UPROPERTY()
	XDLoginType type;

	UPROPERTY()
	FString typeName;

	UPROPERTY()
	int typeValue;

	FXDGLoginTypeModel() = default;
	

	static FString GetName(int Type) {
		if (Type == (int)XDLoginType::TapTap)
		{
			return  "TapTap";
		} else if (Type == (int)XDLoginType::Guest)
		{
			return  "Guest";
		}
		return "";
	}

	FXDGLoginTypeModel(XDLoginType type) : type(type), typeValue((int)type)
	{
		typeName = GetName((int)type);
	}
	
};
