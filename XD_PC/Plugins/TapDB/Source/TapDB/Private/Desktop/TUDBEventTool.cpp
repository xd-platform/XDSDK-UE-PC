#include "TUDBEventTool.h"

#include "TUDeviceInfo.h"
#include "TUHelper.h"


TUDBEventTool * TUDBEventTool::Singleton = nullptr;

TSharedPtr<FJsonObject> TUDBEventTool::GetPresetProperties()
{
	TSharedPtr<FJsonObject> PresetProperties = MakeShareable(new FJsonObject);
	TUHelper::JsonObjectAppend(PresetProperties, GetStaticPresetProperties());
	PresetProperties->SetNumberField("event_index", GetEventIndex());
	PresetProperties->SetStringField("event_uuid", FGuid::NewGuid().ToString());
	PresetProperties->SetStringField("session_uuid", GetSessionUUID());
	return PresetProperties;
}

TSharedPtr<FJsonObject>& TUDBEventTool::GetStaticPresetProperties()
{
	if (!Get().StaticPresetProperties.IsValid())
	{
		TSharedPtr<FJsonObject> Temp = MakeShareable(new FJsonObject);
		Temp->SetNumberField("width", TUDeviceInfo::GetScreenWidth());
		Temp->SetNumberField("height", TUDeviceInfo::GetScreenHeight());
		FString OS = "";
#if PLATFORM_IOS
		OS = "iOS";
#elif PLATFORM_ANDROID
		OS = "Android";
#elif PLATFORM_MAC
		OS = "Mac";
#elif PLATFORM_WINDOWS
		OS = "Windows";
#elif PLATFORM_LINUX
		OS = "Linux";
#endif
		Temp->SetStringField("os", OS);
		Temp->SetStringField("os_version", TUDeviceInfo::GetOSVersion());
		Temp->SetStringField("install_uuid", TUDeviceInfo::GetInstallId());
		Temp->SetStringField("persist_uuid", TUDeviceInfo::GetLoginId());
		Temp->SetStringField("device_id1", TUDeviceInfo::GetLoginId());
		Get().StaticPresetProperties = Temp;
	}
	return Get().StaticPresetProperties;
}

int TUDBEventTool::GetEventIndex()
{
	Get().EventIndex++;
	return Get().EventIndex;
}

void TUDBEventTool::ResetEventIndex()
{
	Get().EventIndex = 0;
}

FString& TUDBEventTool::GetSessionUUID()
{
	if (Get().SessionUUID.Len() <= 0)
	{
		Get().SessionUUID = FGuid::NewGuid().ToString();
	}
	return Get().SessionUUID;
}

TUDBEventTool & TUDBEventTool::Get()
{
	if (Singleton == nullptr)
	{
		Singleton = new TUDBEventTool;
	}
	return *Singleton;
}
