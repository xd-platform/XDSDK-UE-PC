#pragma once


struct XULoginTypeModel
{

	XUType::LoginType type;

	FString typeName;

	int typeValue;

	XULoginTypeModel() = default;
	

	static FString GetName(int Type) {
		if (Type == (int)XUType::TapTap)
		{
			return  "TapTap";
		} else if (Type == (int)XUType::Guest)
		{
			return  "Guest";
		}
		return "";
	}

	XULoginTypeModel(XUType::LoginType Type) : type(Type), typeValue((int)Type)
	{
		typeName = GetName((int)Type);
	}
	
};
