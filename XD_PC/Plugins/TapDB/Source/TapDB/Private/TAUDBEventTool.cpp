#include "TAUDBEventTool.h"

#include "TUDeviceInfo.h"
#include "TDSHelper.h"


TAUDBEventTool * TAUDBEventTool::Singleton = nullptr;

TSharedPtr<FJsonObject> TAUDBEventTool::GetPresetProperties()
{
	TSharedPtr<FJsonObject> PresetProperties = MakeShareable(new FJsonObject);
	TDSHelper::JsonObjectAppend(PresetProperties, GetStaticPresetProperties());
	PresetProperties->SetNumberField("event_index", GetEventIndex());
	PresetProperties->SetStringField("event_uuid", FGuid::NewGuid().ToString());
	PresetProperties->SetStringField("session_uuid", GetSessionUUID());
	return PresetProperties;
}

TSharedPtr<FJsonObject>& TAUDBEventTool::GetStaticPresetProperties()
{
	if (!Get().StaticPresetProperties.IsValid())
	{
		TSharedPtr<FJsonObject> Temp = MakeShareable(new FJsonObject);
		Temp->SetNumberField("width", TUDeviceInfo::GetScreenWidth());
		Temp->SetNumberField("height", TUDeviceInfo::GetScreenHeight());
		Temp->SetStringField("os", TUDeviceInfo::GetPlatform());
		Temp->SetStringField("os_version", TUDeviceInfo::GetOSVersion());
		Temp->SetStringField("install_uuid", TUDeviceInfo::GetInstallId());
		Temp->SetStringField("persist_uuid", TUDeviceInfo::GetLoginId());
		Temp->SetStringField("device_id1", TUDeviceInfo::GetLoginId());
		Get().StaticPresetProperties = Temp;
	}
	return Get().StaticPresetProperties;
}

int TAUDBEventTool::GetEventIndex()
{
	Get().EventIndex++;
	return Get().EventIndex;
}

void TAUDBEventTool::ResetEventIndex()
{
	Get().EventIndex = 0;
}

FString& TAUDBEventTool::GetSessionUUID()
{
	if (Get().SessionUUID.Len() <= 0)
	{
		Get().SessionUUID = FGuid::NewGuid().ToString();
	}
	return Get().SessionUUID;
}

TAUDBEventTool & TAUDBEventTool::Get()
{
	if (Singleton == nullptr)
	{
		Singleton = new TAUDBEventTool;
	}
	return *Singleton;
}
