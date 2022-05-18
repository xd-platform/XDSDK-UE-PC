#include "TUDBEvent.h"

#include "TUDeviceInfo.h"
#include "TUDBEventTool.h"
#include "TUHelper.h"
#include "TUDebuger.h"
#include "TUDBNet.h"
#include "TUDBStorage.h"
#include "VisualLogger/VisualLoggerCustomVersion.h"


using namespace TUDBEventKey;


void TUDBEvent::TrackEvent(const FString& Name, TSharedPtr<FJsonObject> CustomProperties,
                            TSharedPtr<FJsonObject> Properties, TUDBEnum::EventType EventType, TSharedPtr<FJsonObject> SystemParams)
{
	if (Account.IsEmpty() && ClientId.IsEmpty())
	{
		TUDebuger::ErrorLog("Haven't set Appid/Clientid yet, please call onStart first.");
		return;
	}
	if (_Identify.IsEmpty())
	{
		TUDebuger::ErrorLog("Haven't set identify yet, please call autoIdentify or identify first.");
		return;
	}
	TSharedPtr<FJsonObject> DataDic =  GetSystemParams();
	DataDic->SetStringField(TYPE_KEY, "track");
	DataDic->SetStringField(NAME_KEY, Name);
	TUHelper::JsonObjectAppend(DataDic, SystemParams);
	TUHelper::JsonObjectAppend(DataDic, SysProperties);

	TSharedPtr<FJsonObject> MutableProperties = GetSystemParams();
	// if (Name == "device_login" || Name == "user_login") {
	//
	// }
	if (Name == "device_login") {
		MutableProperties->SetNumberField("boot_timestamp", FDateTime::UtcNow().ToUnixTimestamp());
	}
	TUHelper::JsonObjectAppend(MutableProperties, Properties);

	TUHelper::JsonObjectAppend(DataDic, CombinedProperties(MutableProperties, CustomProperties));

	DataDic->SetStringField(OPENID_KEY, GetOpenId());
	
	TUDBNet::SendEvent(EventType, DataDic);
}

void TUDBEvent::InitialEvent(TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject> Properties,
	TUDBEnum::EventType EventType)
{
	auto OperationParams = GetSystemParams();
	TUHelper::JsonObjectAppend(OperationParams, Params);
	OperationParams->SetStringField(TYPE_KEY, "initialise");
	OperationParams->SetObjectField(PROPERTIES_KEY, Properties);
	TUDBNet::SendEvent(EventType, OperationParams);
}

void TUDBEvent::AddEvent(TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject> Properties,
	TUDBEnum::EventType EventType)
{
	auto OperationParams = GetSystemParams();
	TUHelper::JsonObjectAppend(OperationParams, Params);
	OperationParams->SetStringField(TYPE_KEY, "add");
	OperationParams->SetObjectField(PROPERTIES_KEY, Properties);
	TUDBNet::SendEvent(EventType, OperationParams);
}

void TUDBEvent::UpdateEvent(TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject> Properties,
	TUDBEnum::EventType EventType)
{
	auto OperationParams = GetSystemParams();
	TUHelper::JsonObjectAppend(OperationParams, Params);
	OperationParams->SetStringField(TYPE_KEY, "update");
	OperationParams->SetObjectField(PROPERTIES_KEY, Properties);
	TUDBNet::SendEvent(EventType, OperationParams);
}

void TUDBEvent::SetAccount(const FString& _Account)
{
	if (_Account.Len() <= 0 || _Account.Len() > 256)
	{
		TUDebuger::ErrorLog("account is illegal, length should > 0 and <= 256");
		return;
	}
	this->Account = _Account;
	GenerateKey(_Account);
}

void TUDBEvent::SetClientId(const FString& _ClientId)
{
	if (_ClientId.Len() <= 0 || _ClientId.Len() > 256)
	{
		TUDebuger::ErrorLog("clientId is illegal, length should > 0 and <= 256");
		return;
	}
	this->ClientId = _ClientId;
	GenerateKey(_ClientId);
}

FString TUDBEvent::GetOpenId()
{
	return "";
}

bool TUDBEvent::Identify(const FString& identify, const FString& loginType, const TSharedPtr<FJsonObject>& properties)
{
	if (identify.IsEmpty())
	{
		TUDebuger::ErrorLog("invalid UserID!!");
		return false;
	}
	this->_Identify = identify;
	SaveIdentify(identify);

	if (!CommonProperties.IsValid())
	{
		CommonProperties = MakeShareable(new FJsonObject);
	}
	CommonProperties->SetStringField(LOGIN_TYPE_KEY, loginType);
    
	return true;
}

void TUDBEvent::SaveIdentify(const FString& identify)
{
	FString dataKey = Key;
	if (!ClientId.IsEmpty())
	{
		dataKey = FTUDBStorage::ClientIdentifyKey;
	}
	TUDataStorage<FTUDBStorage>::SaveString(dataKey, identify);
}

FString TUDBEvent::GetSavedIdentify()
{
	FString dataKey = Key;
	if (!ClientId.IsEmpty())
	{
		dataKey = FTUDBStorage::ClientIdentifyKey;
	}
	return TUDataStorage<FTUDBStorage>::LoadString(dataKey);
}

bool TUDBEvent::HasSavedIdentify()
{
	return GetSavedIdentify().Len() > 0;
}

void TUDBEvent::ClearIdentify()
{
	_Identify = FString();
	TUDataStorage<FTUDBStorage>::Remove(Key);
}

FString TUDBEvent::GetIdentify()
{
	return _Identify;
}

void TUDBEvent::AutoIdentifyWithProperties(const TSharedPtr<FJsonObject>& properties)
{
	if (!_Identify.IsEmpty())
	{
		TUDebuger::DisplayLog("Already exists identify");
		return;
	}
	auto identy = GetSavedIdentify();
	if (identy.IsEmpty())
	{
		Identify(FGuid::NewGuid().ToString(), FString(), properties);
	} else
	{
		Identify(identy, FString(), properties);
	}
}


void TUDBEvent::GenerateKey(const FString& KeyString)
{
	Key = FString::Printf(TEXT("__tyrantdb__%s__%s__0__"), *KeyString, *GetEventCatogery());
}

TSharedPtr<FJsonObject> TUDBEvent::GetSystemParams()
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	TUHelper::JsonObjectAddNotEmptyString(JsonObject, CLIENTID_KEY, ClientId);
	TUHelper::JsonObjectAddNotEmptyString(JsonObject, INDEX_KEY, Account);
	TUHelper::JsonObjectAddNotEmptyString(JsonObject, GetIdentifyKey(), _Identify);
	TUHelper::JsonObjectAddNotEmptyString(JsonObject, "ip_v6", TUDeviceInfo::GetIpv6());
	
	return JsonObject;
}

TSharedPtr<FJsonObject> TUDBEvent::CombinedProperties(const TSharedPtr<FJsonObject>& Properties,
	const TSharedPtr<FJsonObject>& CustomProperties)
{
	TSharedPtr<FJsonObject> CustomCombinedProperties = MakeShareable(new FJsonObject);

	/* 添加自定义属性*/
	TUHelper::JsonObjectAppend(CustomCombinedProperties, CustomStaticProperties);
	if (CustomDynamicPropertiesCaculator)
	{
		TUHelper::JsonObjectAppend(CustomCombinedProperties, CustomDynamicPropertiesCaculator());
	}
	TUHelper::JsonObjectAppend(CustomCombinedProperties, CustomProperties);

	/* 添加预置属性*/
	TUHelper::JsonObjectAppend(CustomCombinedProperties, TUDBEventTool::GetPresetProperties());
	TUHelper::JsonObjectAppend(CustomCombinedProperties, CommonProperties);
	TUHelper::JsonObjectAppend(CustomCombinedProperties, Properties);

	TSharedPtr<FJsonObject> CombinedProperties = MakeShareable(new FJsonObject);
	CombinedProperties->SetObjectField(PROPERTIES_KEY, CustomCombinedProperties);
	return CombinedProperties;
}

TUDBEventUser::TUDBEventUser(const FString& AccountOrClinentID, bool isAccount)
{
	if (isAccount)
	{
		SetAccount(AccountOrClinentID);
	} else
	{
		SetClientId(AccountOrClinentID);
	}
}

FString TUDBEventUser::GetEventCatogery()
{
	return "user_id";
}

FString TUDBEventUser::GetIdentifyKey()
{
	return "game_user";
}

FString TUDBEventUser::GetOpenId()
{
	return TUHelper::InvokeFunction<FString>("TULoginReflection", "GetOpenID");
}

bool TUDBEventUser::Identify(const FString& identify, const FString& loginType,
	const TSharedPtr<FJsonObject>& properties)
{
	bool newIdentify = TUDBEvent::Identify(identify, loginType, properties);
	if (newIdentify)
	{
		TrackEvent("user_login", properties, nullptr, TUDBEnum::Identify);
	}
	return newIdentify;
}

void TUDBEventUser::SaveIdentify(const FString& identify)
{
}

FString TUDBEventUser::GetSavedIdentify()
{
	return FString();
}

bool TUDBEventUser::HasSavedIdentify()
{
	return false;
}

void TUDBEventUser::ClearIdentify()
{
	TUDBEvent::ClearIdentify();
	if (CommonProperties.IsValid())
	{
		CommonProperties->RemoveField(LOGIN_TYPE_KEY);
	}
}

TUDBEventMobile::TUDBEventMobile(const FString& AccountOrClinentID, bool isAccount)
{
	if (isAccount)
	{
		SetAccount(AccountOrClinentID);
	} else
	{
		SetClientId(AccountOrClinentID);
	}
}

FString TUDBEventMobile::GetEventCatogery()
{
	return "game_mobile";
}

FString TUDBEventMobile::GetIdentifyKey()
{
	return "device_id";
}

bool TUDBEventMobile::Identify(const FString& identify, const FString& loginType, const TSharedPtr<FJsonObject>& properties)
{
	bool newIdentify = TUDBEvent::Identify(identify, loginType, properties);
	if (newIdentify)
	{
		TrackEvent("device_login", properties, nullptr, TUDBEnum::Identify);
	}
	return newIdentify;
}
