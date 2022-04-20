#include "DataStorage.h"

#include <functional>

#include "TDSCrypto.h"

TSharedPtr<FJsonObject> DataStorage::JsonObject = NULL;

static FString DataStoragePath = FPaths::SandboxesDir() + TEXT("/TDS/DataStorageContent");

static FString DataStorageKey = "version1";

TSharedPtr<FJsonObject>& DataStorage::GetJsonObject()
{
	if (JsonObject == NULL)
	{
		FString filePath = DataStoragePath;
		TArray<uint8> data;
		if(FFileHelper::LoadFileToArray(data, *filePath))
		{
			auto JsonStr = TDSCrypto::UTF8Encode(TDSCrypto::AesDecode(data, TDSCrypto::UTF8Encode(DataStorageKey)));
			const TSharedRef<TJsonReader<>>& JsonReader = TJsonReaderFactory<>::Create(JsonStr);
			FJsonSerializer::Deserialize(JsonReader, JsonObject);
			// UE_LOG(LogTemp, Display, TEXT("JsonStr: %s"), *JsonStr);
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
	FString filePath = DataStoragePath;
	FString jsonStr;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&jsonStr);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	auto data = TDSCrypto::AesEncode(TDSCrypto::UTF8Encode(jsonStr), TDSCrypto::UTF8Encode(DataStorageKey));
	FFileHelper::SaveArrayToFile(data, *filePath);
}







