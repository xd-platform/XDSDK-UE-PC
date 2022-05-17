#include "TauDBEvent.h"

#include "TUDeviceInfo.h"
#include "TAUDBEventTool.h"
#include "TUHelper.h"
#include "TUDebuger.h"
#include "TAUDBNet.h"
#include "TauDBStorage.h"
#include "VisualLogger/VisualLoggerCustomVersion.h"


using namespace TauDBEventKey;


void TauDBEvent::TrackEvent(const FString& Name, TSharedPtr<FJsonObject> CustomProperties,
                            TSharedPtr<FJsonObject> Properties, TauDBEnum::EventType EventType, TSharedPtr<FJsonObject> SystemParams)
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
	
	TAUDBNet::SendEvent(EventType, DataDic);
}

void TauDBEvent::InitialEvent(TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject> Properties,
	TauDBEnum::EventType EventType)
{
	auto OperationParams = GetSystemParams();
	TUHelper::JsonObjectAppend(OperationParams, Params);
	OperationParams->SetStringField(TYPE_KEY, "initialise");
	OperationParams->SetObjectField(PROPERTIES_KEY, Properties);
	TAUDBNet::SendEvent(EventType, OperationParams);
}

void TauDBEvent::AddEvent(TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject> Properties,
	TauDBEnum::EventType EventType)
{
	auto OperationParams = GetSystemParams();
	TUHelper::JsonObjectAppend(OperationParams, Params);
	OperationParams->SetStringField(TYPE_KEY, "add");
	OperationParams->SetObjectField(PROPERTIES_KEY, Properties);
	TAUDBNet::SendEvent(EventType, OperationParams);
}

void TauDBEvent::UpdateEvent(TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject> Properties,
	TauDBEnum::EventType EventType)
{
	auto OperationParams = GetSystemParams();
	TUHelper::JsonObjectAppend(OperationParams, Params);
	OperationParams->SetStringField(TYPE_KEY, "update");
	OperationParams->SetObjectField(PROPERTIES_KEY, Properties);
	TAUDBNet::SendEvent(EventType, OperationParams);
}

void TauDBEvent::SetAccount(const FString& _Account)
{
	if (_Account.Len() <= 0 || _Account.Len() > 256)
	{
		TUDebuger::ErrorLog("account is illegal, length should > 0 and <= 256");
		return;
	}
	this->Account = _Account;
	GenerateKey(_Account);
}

void TauDBEvent::SetClientId(const FString& _ClientId)
{
	if (_ClientId.Len() <= 0 || _ClientId.Len() > 256)
	{
		TUDebuger::ErrorLog("clientId is illegal, length should > 0 and <= 256");
		return;
	}
	this->ClientId = _ClientId;
	GenerateKey(_ClientId);
}

FString TauDBEvent::GetOpenId()
{
	return "";
}

bool TauDBEvent::Identify(const FString& identify, const FString& loginType, const TSharedPtr<FJsonObject>& properties)
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

void TauDBEvent::SaveIdentify(const FString& identify)
{
	FString dataKey = Key;
	if (!ClientId.IsEmpty())
	{
		dataKey = FTauDBStorage::ClientIdentifyKey;
	}
	DataStorage<FTauDBStorage>::SaveString(dataKey, identify);
}

FString TauDBEvent::GetSavedIdentify()
{
	FString dataKey = Key;
	if (!ClientId.IsEmpty())
	{
		dataKey = FTauDBStorage::ClientIdentifyKey;
	}
	return DataStorage<FTauDBStorage>::LoadString(dataKey);
}

bool TauDBEvent::HasSavedIdentify()
{
	return GetSavedIdentify().Len() > 0;
}

void TauDBEvent::ClearIdentify()
{
	_Identify = FString();
	DataStorage<FTauDBStorage>::Remove(Key);
}

FString TauDBEvent::GetIdentify()
{
	return _Identify;
}

void TauDBEvent::AutoIdentifyWithProperties(const TSharedPtr<FJsonObject>& properties)
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


void TauDBEvent::GenerateKey(const FString& KeyString)
{
	Key = FString::Printf(TEXT("__tyrantdb__%s__%s__0__"), *KeyString, *GetEventCatogery());
}

TSharedPtr<FJsonObject> TauDBEvent::GetSystemParams()
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	TUHelper::JsonObjectAddNotEmptyString(JsonObject, CLIENTID_KEY, ClientId);
	TUHelper::JsonObjectAddNotEmptyString(JsonObject, INDEX_KEY, Account);
	TUHelper::JsonObjectAddNotEmptyString(JsonObject, GetIdentifyKey(), _Identify);
	TUHelper::JsonObjectAddNotEmptyString(JsonObject, "ip_v6", TUDeviceInfo::GetIpv6());
	
	return JsonObject;
}

TSharedPtr<FJsonObject> TauDBEvent::CombinedProperties(const TSharedPtr<FJsonObject>& Properties,
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
	TUHelper::JsonObjectAppend(CustomCombinedProperties, TAUDBEventTool::GetPresetProperties());
	TUHelper::JsonObjectAppend(CustomCombinedProperties, CommonProperties);
	TUHelper::JsonObjectAppend(CustomCombinedProperties, Properties);

	TSharedPtr<FJsonObject> CombinedProperties = MakeShareable(new FJsonObject);
	CombinedProperties->SetObjectField(PROPERTIES_KEY, CustomCombinedProperties);
	return CombinedProperties;
}

TauDBEventUser::TauDBEventUser(const FString& AccountOrClinentID, bool isAccount)
{
	if (isAccount)
	{
		SetAccount(AccountOrClinentID);
	} else
	{
		SetClientId(AccountOrClinentID);
	}
}

FString TauDBEventUser::GetEventCatogery()
{
	return "user_id";
}

FString TauDBEventUser::GetIdentifyKey()
{
	return "game_user";
}

FString TauDBEventUser::GetOpenId()
{
	return TUHelper::InvokeFunction<FString>("TapLoginReflection", "GetOpenID");
}

bool TauDBEventUser::Identify(const FString& identify, const FString& loginType,
	const TSharedPtr<FJsonObject>& properties)
{
	bool newIdentify = TauDBEvent::Identify(identify, loginType, properties);
	if (newIdentify)
	{
		TrackEvent("user_login", properties, nullptr, TauDBEnum::Identify);
	}
	return newIdentify;
}

void TauDBEventUser::SaveIdentify(const FString& identify)
{
}

FString TauDBEventUser::GetSavedIdentify()
{
	return FString();
}

bool TauDBEventUser::HasSavedIdentify()
{
	return false;
}

void TauDBEventUser::ClearIdentify()
{
	TauDBEvent::ClearIdentify();
	if (CommonProperties.IsValid())
	{
		CommonProperties->RemoveField(LOGIN_TYPE_KEY);
	}
}

TauDBEventMobile::TauDBEventMobile(const FString& AccountOrClinentID, bool isAccount)
{
	if (isAccount)
	{
		SetAccount(AccountOrClinentID);
	} else
	{
		SetClientId(AccountOrClinentID);
	}
}

FString TauDBEventMobile::GetEventCatogery()
{
	return "game_mobile";
}

FString TauDBEventMobile::GetIdentifyKey()
{
	return "device_id";
}

bool TauDBEventMobile::Identify(const FString& identify, const FString& loginType, const TSharedPtr<FJsonObject>& properties)
{
	bool newIdentify = TauDBEvent::Identify(identify, loginType, properties);
	if (newIdentify)
	{
		TrackEvent("device_login", properties, nullptr, TauDBEnum::Identify);
	}
	return newIdentify;
}
