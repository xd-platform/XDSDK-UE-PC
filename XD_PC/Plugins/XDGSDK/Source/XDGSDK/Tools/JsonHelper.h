#pragma once
#include "JsonObjectConverter.h"

class JsonHelper
{
public:
	static FString GetJsonString(const TSharedPtr<FJsonObject>& JsonObject)
	{
		FString OutputString;
		if (JsonObject == nullptr)
		{
			return "";
		}
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
		FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
		return OutputString;
	}

	template <typename UStructType>
	static FString GetJsonString(const TSharedPtr<UStructType>& value)
	{
		TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(*value.Get());
		return GetJsonString(JsonObject);
	}

	template <typename UStructType>
	static FString GetJsonString(const UStructType& value)
	{
		TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(value);
		return GetJsonString(JsonObject);
	}

	
	static TSharedPtr<FJsonObject> GetJsonObject(const FString& JsonString)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
		if (!FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			JsonObject = MakeShareable(new FJsonObject);
		}
		return JsonObject;
	}

	template <typename UStructType>
	static TSharedPtr<FJsonObject> GetJsonObject(const TSharedPtr<UStructType>& value)
	{
		TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(*value.Get());
		return JsonObject;
	}

	template <typename UStructType>
	static TSharedPtr<FJsonObject> GetJsonObject(const UStructType& value)
	{
		TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(value);
		return JsonObject;
	}

	template <typename UStructType>
	static TSharedPtr<UStructType> GetUStruct(const TSharedPtr<FJsonObject>& JsonObject)
	{
		TSharedPtr<UStructType> value = MakeShareable(new UStructType);
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), value.Get());
		return value;
	}

	template <typename UStructType>
	static TSharedPtr<UStructType> GetUStruct(const FString& JsonString)
	{
		return GetUStruct<UStructType>(GetJsonObject(JsonString));
	}
};
