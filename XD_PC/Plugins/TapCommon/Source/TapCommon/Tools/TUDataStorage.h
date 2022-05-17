#pragma once
#include "JsonObjectConverter.h"
#include "TUJsonHelper.h"
#include "TUCrypto.h"

template <typename StructName>
class TUDataStorage
{
public:
	

	static void SaveString(const FString& key, const FString& value, bool needSaveLocal = true)
	{
		GetJsonObject()->SetStringField(key, value);
		if (needSaveLocal){ SaveToFile(); }
	}
	static FString LoadString(const FString& key)
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

	static void SaveBool(const FString& key, bool value, bool needSaveLocal = true)
	{
		GetJsonObject()->SetBoolField(key, value);
		if (needSaveLocal){ SaveToFile(); }
	}
	
	static bool LoadBool(const FString& key)
	{
		bool Value;
		if (GetJsonObject()->TryGetBoolField(key, Value))
		{
			return Value;
		} else
		{
			return false;
		}
	}

	static void SaveNumber(const FString& key, double value, bool needSaveLocal = true)
	{
		GetJsonObject()->SetNumberField(key, value);
		if (needSaveLocal){ SaveToFile(); }
	}
	
	static double LoadNumber(const FString& key)
	{
		double Value;
		if (GetJsonObject()->TryGetNumberField(key, Value))
		{
			return Value;
		} else
		{
			return 0;
		}
	}

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

	static void Remove(const FString& key, bool needSaveLocal = true)
	{
		GetJsonObject()->RemoveField(key);
		if (needSaveLocal){ SaveToFile(); }
	}

	static void SaveToFile()
	{
		FString filePath = DataStoragePath();
		FString jsonStr = TUJsonHelper::GetJsonString(JsonObject);
		auto data = TUCrypto::AesEncode(TUCrypto::UTF8Encode(jsonStr), TUCrypto::UTF8Encode(DataStorageKey()));
		FFileHelper::SaveArrayToFile(data, *filePath);
	}

private:
	
	static TSharedPtr<FJsonObject>& GetJsonObject()
	{
		if (JsonObject == nullptr)
		{
			FString filePath = DataStoragePath();
			TArray<uint8> data;
			if(FFileHelper::LoadFileToArray(data, *filePath))
			{
				auto JsonStr = TUCrypto::UTF8Encode(TUCrypto::AesDecode(data, TUCrypto::UTF8Encode(DataStorageKey())));
				JsonObject = TUJsonHelper::GetJsonObject(JsonStr);
				// UE_LOG(LogTemp, Display, TEXT("JsonStr: %s"), *JsonStr);
			}
		}
		if (JsonObject == nullptr)
		{
			JsonObject = MakeShareable(new FJsonObject);
		}
		return JsonObject;
	}

	static TSharedPtr<FJsonObject> JsonObject;
	static FString DataStoragePath()
	{
		return FPaths::SandboxesDir() + TEXT("/DataStorage/") + StructName::StaticStruct()->GetName();
	};
	
	static FString DataStorageKey()
	{
		return StructName::StaticStruct()->GetName() + TEXT("Key");
	}
};

template <typename StructName>
TSharedPtr<FJsonObject> TUDataStorage<StructName>::JsonObject = nullptr;

