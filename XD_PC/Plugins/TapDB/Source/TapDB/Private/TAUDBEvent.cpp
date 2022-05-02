#include "TAUDBEvent.h"

#include "DeviceInfo.h"
#include "TAUDBEventTool.h"
#include "TDSHelper.h"
#include "TDUDebuger.h"
#include "TAUDBNet.h"
#include "TAUDBStorage.h"


static FString const INDEX_KEY                = "index";
static FString const NAME_KEY                 = "name";
static FString const PROPERTIES_KEY           = "properties";
static FString const CUSTOM_PROPERTIES_KEY    = "customProperties";

static FString const MODULE_KEY               = "module";
static FString const TYPE_KEY                 = "type";

static FString const OPENID_KEY               = "open_id";
static FString const CLIENTID_KEY             = "client_id";
static FString const LOGIN_TYPE_KEY           = "login_type";


TAUDBEvent::TAUDBEvent(const FString& AccountOrClinentID, bool isAccount)
{
	if (isAccount)
	{
		SetAccount(AccountOrClinentID);
	} else
	{
		SetClientId(AccountOrClinentID);
	}
}

void TAUDBEvent::TrackEvent(const FString& Name, TSharedPtr<FJsonObject> CustomProperties,
                            TSharedPtr<FJsonObject> Properties, TAUDBEnum::EventType EventType, TSharedPtr<FJsonObject> SystemParams)
{
	if (Account.IsEmpty() && ClientId.IsEmpty())
	{
		TDUDebuger::ErrorLog("Haven't set Appid/Clientid yet, please call onStart first.");
		return;
	}
	if (_Identify.IsEmpty())
	{
		TDUDebuger::ErrorLog("Haven't set identify yet, please call autoIdentify or identify first.");
		return;
	}
	TSharedPtr<FJsonObject> DataDic =  GetSystemParams();
	DataDic->SetStringField(TYPE_KEY, "track");
	DataDic->SetStringField(NAME_KEY, Name);
	TDSHelper::JsonObjectAppend(DataDic, SystemParams);
	TDSHelper::JsonObjectAppend(DataDic, SysProperties);

	TSharedPtr<FJsonObject> MutableProperties =  GetSystemParams();
	if (Name == "device_login" || Name == "user_login")
	{
		
	}

	if (Name == "device_login")
	{
		
	}
	TDSHelper::JsonObjectAppend(MutableProperties, Properties);

	TDSHelper::JsonObjectAppend(DataDic, CombinedProperties(MutableProperties, CustomProperties));

	DataDic->SetStringField(OPENID_KEY, GetOpenId());
	
	TAUDBNet::SendEvent(EventType, DataDic);
}

void TAUDBEvent::InitialEvent(TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject> Properties,
	TAUDBEnum::EventType EventType)
{
	auto OperationParams = GetSystemParams();
	TDSHelper::JsonObjectAppend(OperationParams, Params);
	OperationParams->SetStringField(TYPE_KEY, "initialise");
	OperationParams->SetObjectField(PROPERTIES_KEY, Properties);
	TAUDBNet::SendEvent(EventType, OperationParams);
}

void TAUDBEvent::AddEvent(TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject> Properties,
	TAUDBEnum::EventType EventType)
{
	auto OperationParams = GetSystemParams();
	TDSHelper::JsonObjectAppend(OperationParams, Params);
	OperationParams->SetStringField(TYPE_KEY, "add");
	OperationParams->SetObjectField(PROPERTIES_KEY, Properties);
	TAUDBNet::SendEvent(EventType, OperationParams);
}

void TAUDBEvent::UpdateEvent(TSharedPtr<FJsonObject> Params, TSharedPtr<FJsonObject> Properties,
	TAUDBEnum::EventType EventType)
{
	auto OperationParams = GetSystemParams();
	TDSHelper::JsonObjectAppend(OperationParams, Params);
	OperationParams->SetStringField(TYPE_KEY, "update");
	OperationParams->SetObjectField(PROPERTIES_KEY, Properties);
	TAUDBNet::SendEvent(EventType, OperationParams);
}

void TAUDBEvent::SetAccount(const FString& _Account)
{
	if (_Account.Len() <= 0 || _Account.Len() > 256)
	{
		TDUDebuger::ErrorLog("account is illegal, length should > 0 and <= 256");
		return;
	}
	this->Account = _Account;
	GenerateKey(_Account);
}

void TAUDBEvent::SetClientId(const FString& _ClientId)
{
	if (_ClientId.Len() <= 0 || _ClientId.Len() > 256)
	{
		TDUDebuger::ErrorLog("clientId is illegal, length should > 0 and <= 256");
		return;
	}
	this->ClientId = _ClientId;
	GenerateKey(_ClientId);
}

FString TAUDBEvent::GetOpenId()
{
	return "";
}

bool TAUDBEvent::Identify(const FString& identify, const FString& loginType, const TSharedPtr<FJsonObject>& properties)
{
	if (identify.IsEmpty())
	{
		TDUDebuger::ErrorLog("nvalid UserID!!");
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

void TAUDBEvent::SaveIdentify(const FString& identify)
{
	FString dataKey = Key;
	if (!ClientId.IsEmpty())
	{
		dataKey = FTAUDBStorage::ClientIdentifyKey;
	}
	DataStorage<FTAUDBStorage>::SaveString(dataKey, identify);
}

FString TAUDBEvent::GetSavedIdentify()
{
	FString dataKey = Key;
	if (!ClientId.IsEmpty())
	{
		dataKey = FTAUDBStorage::ClientIdentifyKey;
	}
	return DataStorage<FTAUDBStorage>::LoadString(dataKey);
}

bool TAUDBEvent::HasSavedIdentify()
{
	return GetSavedIdentify().Len() > 0;
}

void TAUDBEvent::ClearIdentify()
{
	_Identify = FString();
	DataStorage<FTAUDBStorage>::Remove(Key);
}


void TAUDBEvent::GenerateKey(const FString& KeyString)
{
	Key = FString::Printf(TEXT("__tyrantdb__%s__%s__0__"), *KeyString, *GetEventCatogery());
}

TSharedPtr<FJsonObject> TAUDBEvent::GetSystemParams()
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	TDSHelper::JsonObjectAddNotEmptyString(JsonObject, CLIENTID_KEY, ClientId);
	TDSHelper::JsonObjectAddNotEmptyString(JsonObject, INDEX_KEY, Account);
	TDSHelper::JsonObjectAddNotEmptyString(JsonObject, GetIdentifyKey(), _Identify);
	TDSHelper::JsonObjectAddNotEmptyString(JsonObject, "ip_v6", DeviceInfo::GetIpv6());
	
	return JsonObject;
}

TSharedPtr<FJsonObject> TAUDBEvent::CombinedProperties(const TSharedPtr<FJsonObject>& Properties,
	const TSharedPtr<FJsonObject>& CustomProperties)
{
	TSharedPtr<FJsonObject> CustomCombinedProperties = MakeShareable(new FJsonObject);

	/* 添加自定义属性*/
	TDSHelper::JsonObjectAppend(CustomCombinedProperties, CustomStaticProperties);
	if (CustomDynamicPropertiesCaculator)
	{
		TDSHelper::JsonObjectAppend(CustomCombinedProperties, CustomDynamicPropertiesCaculator());
	}
	TDSHelper::JsonObjectAppend(CustomCombinedProperties, CustomProperties);

	/* 添加预置属性*/
	TDSHelper::JsonObjectAppend(CustomCombinedProperties, TAUDBEventTool::GetPresetProperties());
	TDSHelper::JsonObjectAppend(CustomCombinedProperties, CommonProperties);
	TDSHelper::JsonObjectAppend(CustomCombinedProperties, Properties);

	TSharedPtr<FJsonObject> CombinedProperties = MakeShareable(new FJsonObject);
	CombinedProperties->SetObjectField(PROPERTIES_KEY, CustomCombinedProperties);
	return CombinedProperties;
}

FString TAUDBEventUser::GetEventCatogery()
{
	return "user_id";
}

FString TAUDBEventUser::GetIdentifyKey()
{
	return "game_user";
}

FString TAUDBEventUser::GetOpenId()
{
	return TDSHelper::InvokeFunction<FString>("TapLoginReflection", "GetOpenID");
}

bool TAUDBEventUser::Identify(const FString& identify, const FString& loginType,
	const TSharedPtr<FJsonObject>& properties)
{
	bool newIdentify = TAUDBEvent::Identify(identify, loginType, properties);
	if (newIdentify)
	{
		TrackEvent("user_login", properties, nullptr, TAUDBEnum::Identify);
	}
	return newIdentify;
}

void TAUDBEventUser::SaveIdentify(const FString& identify)
{
}

FString TAUDBEventUser::GetSavedIdentify()
{
	return FString();
}

bool TAUDBEventUser::HasSavedIdentify()
{
	return false;
}

void TAUDBEventUser::ClearIdentify()
{
	TAUDBEvent::ClearIdentify();
	if (CommonProperties.IsValid())
	{
		CommonProperties->RemoveField(LOGIN_TYPE_KEY);
	}
}

FString TAUDBEventMobile::GetEventCatogery()
{
	return "game_mobile";
}

FString TAUDBEventMobile::GetIdentifyKey()
{
	return "device_id";
}

bool TAUDBEventMobile::Identify(const FString& identify, const FString& loginType, const TSharedPtr<FJsonObject>& properties)
{
	bool newIdentify = TAUDBEvent::Identify(identify, loginType, properties);
	if (newIdentify)
	{
		TrackEvent("device_login", properties, nullptr, TAUDBEnum::Identify);
	}
	return newIdentify;
}
