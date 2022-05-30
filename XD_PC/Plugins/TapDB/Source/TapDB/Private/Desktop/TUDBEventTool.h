#pragma once

class TUDBEventTool
{
public:

	static TSharedPtr<FJsonObject> GetPresetProperties();
	
	static int GetEventIndex();
	static void ResetEventIndex();
	static FString& GetSessionUUID();
	


private:
	static TUDBEventTool *Singleton;
	static TUDBEventTool& Get();
	TSharedPtr<FJsonObject> StaticPresetProperties;
	int EventIndex = 0;
	FString SessionUUID;

	static TSharedPtr<FJsonObject>& GetStaticPresetProperties();

};
