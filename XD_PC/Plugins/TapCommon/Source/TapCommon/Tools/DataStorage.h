#pragma once
#include "JsonObjectConverter.h"

// template <typename StructName>
class TAPCOMMON_API DataStorage
{
public:
	

	static void SaveString(const FString& key, const FString& value, bool needSaveLocal = true);
	static FString LoadString(const FString& key);

	static void SaveBool(const FString& key, bool value, bool needSaveLocal = true);
	static bool LoadBool(const FString& key);

	template <typename StructType>
	static void SaveStruct(const FString& key, const TSharedPtr<StructType>& value, bool needSaveLocal = true)
	{
		TSharedPtr<FJsonObject> jsonObject = FJsonObjectConverter::UStructToJsonObject(*value.Get());
		GetJsonObject()->SetObjectField(key, jsonObject);
		if (needSaveLocal){ SaveToFile(); }
	}

	template <typename StructType>
	static void SaveStruct(const FString& key, const StructType& value, bool needSaveLocal = true)
	{
		TSharedPtr<FJsonObject> jsonObject = FJsonObjectConverter::UStructToJsonObject(value);
		GetJsonObject()->SetObjectField(key, jsonObject);
		if (needSaveLocal){ SaveToFile(); }
	}

	template <typename StructType>
	static TSharedPtr<StructType> LoadStruct(const FString& key)
	{
		const TSharedPtr<FJsonObject>* jsonObject;
		if (!GetJsonObject()->TryGetObjectField(key, jsonObject))
		{
			return nullptr;
		}
		TSharedPtr<StructType> value = MakeShareable(new StructType);
		FJsonObjectConverter::JsonObjectToUStruct(jsonObject->ToSharedRef(), value.Get());
		return value;
	}

	static void Remove(const FString& key, bool needSaveLocal = true);

	static void SaveToFile();

private:
	
	static TSharedPtr<FJsonObject> JsonObject;
	static TSharedPtr<FJsonObject>& GetJsonObject();
};
