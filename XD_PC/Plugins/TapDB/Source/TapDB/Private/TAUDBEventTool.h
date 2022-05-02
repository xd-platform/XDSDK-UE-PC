#pragma once

class TAUDBEventTool
{
public:

	static TSharedPtr<FJsonObject> GetPresetProperties();
	
	static int GetEventIndex();
	static void ResetEventIndex();
	static FString& GetSessionUUID();
	


private:
	static TAUDBEventTool *Singleton;
	static TAUDBEventTool& Get();
	TSharedPtr<FJsonObject> StaticPresetProperties;
	int EventIndex = 0;
	FString SessionUUID;

	static TSharedPtr<FJsonObject>& GetStaticPresetProperties();

};
