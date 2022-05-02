#include "TAUDBEventTool.h"

#include "DeviceInfo.h"
#include "TDSHelper.h"


TAUDBEventTool * TAUDBEventTool::Singleton = nullptr;

TSharedPtr<FJsonObject> TAUDBEventTool::GetPresetProperties()
{
	TSharedPtr<FJsonObject> PresetProperties;
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
		Temp->SetNumberField("width", DeviceInfo::GetScreenWidth());
		Temp->SetNumberField("height", DeviceInfo::GetScreenHeight());
		Temp->SetStringField("os", DeviceInfo::GetPlatform());
		Temp->SetStringField("os_version", DeviceInfo::GetOSVersion());
		Temp->SetStringField("install_uuid", DeviceInfo::GetInstallId());
		Temp->SetStringField("persist_uuid", DeviceInfo::GetLoginId());
		Temp->SetStringField("device_id1", DeviceInfo::GetLoginId());
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
