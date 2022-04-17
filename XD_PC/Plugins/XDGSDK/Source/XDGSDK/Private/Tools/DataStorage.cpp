#include "DataStorage.h"

#include <functional>

TSharedPtr<FJsonObject> DataStorage::JsonObject = NULL;


FString DataStoragePath()
{
	FString filePath = FPaths::SandboxesDir() + TEXT("/TDS/DataStorage.json");
	return filePath;
}

TSharedPtr<FJsonObject>& DataStorage::GetJsonObject()
{
	if (JsonObject == NULL)
	{
		FString filePath = DataStoragePath();
		FString JsonStr;
		if(FFileHelper::LoadFileToString(JsonStr, *filePath))
		{
			const TSharedRef<TJsonReader<>>& JsonReader = TJsonReaderFactory<>::Create(JsonStr);
			FJsonSerializer::Deserialize(JsonReader, JsonObject);
		}
	}
	if (JsonObject == NULL)
	{
		JsonObject = MakeShareable(new FJsonObject);
	}
	return JsonObject;
}

void DataStorage::SaveString(const FString& key, const FString& value, bool needSaveLocal)
{
	GetJsonObject()->SetStringField(key, value);
	if (needSaveLocal){ SaveToFile(); }
}

FString DataStorage::LoadString(const FString& key)
{
	return GetJsonObject()->GetStringField(key);
}

void DataStorage::SaveToFile()
{
	FString filePath = DataStoragePath();
	FString jsonStr;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&jsonStr);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	FFileHelper::SaveStringToFile(jsonStr, *filePath);
}







