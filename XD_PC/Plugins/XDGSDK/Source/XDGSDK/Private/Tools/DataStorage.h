#pragma once
#include "JsonObjectConverter.h"

class DataStorage
{
public:
	

	static void SaveString(const FString& key, const FString& value, bool needSaveLocal = true);
	static FString LoadString(const FString& key);

	template <typename StructType>
	static void SaveStruct(const FString& key, const TSharedPtr<StructType>& value, bool needSaveLocal = true)
	{
		TSharedPtr<FJsonObject> jsonObject = FJsonObjectConverter::UStructToJsonObject(*value.Get());
		GetJsonObject()->SetObjectField(key, jsonObject);
		if (needSaveLocal){ SaveToFile(); }
	}

	template <typename StructType>
	static TSharedPtr<StructType> LoadStruct(const FString& key)
	{
		TSharedPtr<FJsonObject> jsonObject = GetJsonObject()-> GetObjectField(key);
		if (jsonObject == nullptr)
		{
			return nullptr;
		}
		TSharedPtr<StructType> value = MakeShareable(new StructType);
		FJsonObjectConverter::JsonObjectToUStruct(jsonObject.ToSharedRef(), value.Get());
		return value;
	}

	static void SaveToFile();

private:
	
	static TSharedPtr<FJsonObject> JsonObject;
	static TSharedPtr<FJsonObject>& GetJsonObject();
};
