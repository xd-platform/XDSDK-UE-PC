#pragma once

class DataStorage
{
public:
	

	static void SaveString(const FString& key, const FString& value, bool needSaveLocal = false);
	static FString LoadString(const FString& key);

	static void SaveToFile();

private:
	
	static TSharedPtr<FJsonObject> JsonObject;
	static TSharedPtr<FJsonObject>& GetJsonObject();
};
