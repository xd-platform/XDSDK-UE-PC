#include "DataStorage.h"

#include "JsonHelper.h"
#include "TDSCrypto.h"

TSharedPtr<FJsonObject> DataStorage::JsonObject = nullptr;

static FString DataStoragePath = FPaths::SandboxesDir() + TEXT("/TDS/DataStorageContent");

static FString DataStorageKey = "version1";

TSharedPtr<FJsonObject>& DataStorage::GetJsonObject()
{
	if (JsonObject == nullptr)
	{
		FString filePath = DataStoragePath;
		TArray<uint8> data;
		if(FFileHelper::LoadFileToArray(data, *filePath))
		{
			auto JsonStr = TDSCrypto::UTF8Encode(TDSCrypto::AesDecode(data, TDSCrypto::UTF8Encode(DataStorageKey)));
			JsonObject = JsonHelper::GetJsonObject(JsonStr);
			// UE_LOG(LogTemp, Display, TEXT("JsonStr: %s"), *JsonStr);
		}
	}
	if (JsonObject == nullptr)
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
	FString String;
	if (GetJsonObject()->TryGetStringField(key, String))
	{
		return String;
	} else
	{
		return FString();
	}
	
	
}

void DataStorage::Remove(const FString& key, bool needSaveLocal)
{
	GetJsonObject()->RemoveField(key);
	if (needSaveLocal){ SaveToFile(); }
}

void DataStorage::SaveToFile()
{
	FString filePath = DataStoragePath;
	FString jsonStr = JsonHelper::GetJsonString(JsonObject);
	auto data = TDSCrypto::AesEncode(TDSCrypto::UTF8Encode(jsonStr), TDSCrypto::UTF8Encode(DataStorageKey));
	FFileHelper::SaveArrayToFile(data, *filePath);
}







