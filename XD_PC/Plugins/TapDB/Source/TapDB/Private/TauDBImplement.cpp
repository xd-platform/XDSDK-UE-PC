#include "TauDBImplement.h"

#include "DeviceInfo.h"
#include "JsonHelper.h"
#include "TauDB.h"
#include "TAUDBNet.h"
#include "TauDBStorage.h"
#include "TDSHelper.h"

TauDBImplement * TauDBImplement::Singleton = nullptr;
FString TauDBImplement::First_login_type = "";
FString TauDBImplement::First_open_id = "";
FString TauDBImplement::TAPDB_VERSION = "3.0.9";


TauDBImplement & TauDBImplement::Get()
{
	if (Singleton == nullptr)
	{
		Singleton = new TauDBImplement;
	}
	return *Singleton;
}

void TauDBImplement::Init(const TauDBInitConfig& Config, bool isAccount)
{
	TapDBEventMobile = MakeShareable(new TauDBEventMobile(Config.AppIdOrClientId, isAccount));
	TapDBEventUser = MakeShareable(new TauDBEventUser(Config.AppIdOrClientId, isAccount));

	TSharedPtr<FJsonObject> CommonProperties = MakeShareable(new FJsonObject);
	if (Config.Channel.Len() > 0 && Config.Channel.Len() <= 256)
	{
		CommonProperties->SetStringField("channel", Config.Channel);
	}
	if (Config.GameVersion.Len() > 0 && Config.GameVersion.Len() <= 256)
	{
		CommonProperties->SetStringField("app_version", Config.GameVersion);
	} else
	{
		CommonProperties->SetStringField("app_version", DeviceInfo::GetProjectVersion());
	}
	CommonProperties->SetStringField("sdk_version", TAPDB_VERSION);

	TapDBEventMobile->CommonProperties = CommonProperties;
	
	StartTime = MakeShareable(new FDateTime(FDateTime::Now()));
	RegisterCoreDelegate();
	StartOperation(Config.Properties);

	TapDBEventUser->CommonProperties = CommonProperties;
	TSharedPtr<FJsonObject> DeviceIdProperty = MakeShareable(new FJsonObject);
	DeviceIdProperty->SetStringField(TapDBEventMobile->GetIdentifyKey(), TapDBEventMobile->GetIdentify());
	TapDBEventUser->SysProperties = DeviceIdProperty;
}

void TauDBImplement::SetUser(const FString& UserId, TSharedPtr<FJsonObject> Properties, const FString& LoginType)
{
	if (!CheckStringParam(UserId, "UserId"))
	{
		return;
	}
	TapDBEventUser->Identify(UserId, LoginType, Properties);

	TSharedPtr<FJsonObject> InitialOperationProperties = MakeShareable(new FJsonObject);
	if (First_login_type.IsEmpty())
	{
		First_login_type = LoginType;
		InitialOperationProperties->SetStringField("first_login_type", LoginType);
	}

	TSharedPtr<FJsonObject> UpdateOperationProperties = MakeShareable(new FJsonObject);
	UpdateOperationProperties->SetBoolField("has_user", true);
	UpdateOperationProperties->SetStringField("current_login_type", LoginType);

	if (LoginType == TauDBLoginType::TapTap)
	{
		if (First_open_id.IsEmpty())
		{
			FString OpenId = TapDBEventUser->GetOpenId();
			First_open_id = OpenId;
			InitialOperationProperties->SetStringField("first_open_id", OpenId);
		}
	}

	TDSHelper::JsonObjectAppend(InitialOperationProperties, Properties);
	
	TDSHelper::JsonObjectAppend(UpdateOperationProperties, Properties);
	TDSHelper::JsonObjectAppend(UpdateOperationProperties, CustomOperationProperties(nullptr));

	if (InitialOperationProperties->Values.Num() > 0)
	{
		TDSHelper::JsonObjectAppend(InitialOperationProperties, CustomOperationProperties(nullptr));
		TapDBEventMobile->InitialEvent(nullptr, InitialOperationProperties, TauDBEnum::Normal);
	}
	TapDBEventMobile->UpdateEvent(nullptr, UpdateOperationProperties, TauDBEnum::Normal);

	double PlayTime = DataStorage<FTauDBStorage>::LoadNumber(FTauDBStorage::TapDBPlayTime);
	if (PlayTime > 0)
	{
		SendPlayTime(PlayTime);
		DataStorage<FTauDBStorage>::Remove(FTauDBStorage::TapDBPlayTime);
	}
	
}

void TauDBImplement::ClearUser()
{
	TapDBEventUser->ClearIdentify();
}

TSharedPtr<FJsonObject> TauDBImplement::GetStartInfo()
{
	return TapDBEventMobile->CommonProperties;
}

FString TauDBImplement::GetDeviceId()
{
	return TapDBEventMobile->GetIdentify();
}

void TauDBImplement::SetLevel(int Level)
{
	if (!CheckIdentify())
	{
		return;
	}
	if (Level < 0)
	{
		TDUDebuger::ErrorLog("level is illegal, should >= 0");
		return;
	}
	TSharedPtr<FJsonObject> LevelProperties = MakeShareable(new FJsonObject);
	LevelProperties->SetNumberField("level", Level);
	TDSHelper::JsonObjectAppend(LevelProperties, CustomOperationProperties(nullptr));
	TapDBEventUser->UpdateEvent(nullptr, LevelProperties, TauDBEnum::Normal);
}

void TauDBImplement::SetServer(const FString& Server)
{
	if (!CheckIdentify() || !CheckStringParam(Server, "Server"))
	{
		return;
	}
	
	TSharedPtr<FJsonObject> ServerInitialiseProperties = MakeShareable(new FJsonObject);
	ServerInitialiseProperties->SetStringField("first_server", Server);
	TDSHelper::JsonObjectAppend(ServerInitialiseProperties, CustomOperationProperties(nullptr));
	TapDBEventUser->InitialEvent(nullptr, ServerInitialiseProperties, TauDBEnum::Normal);

	TSharedPtr<FJsonObject> ServerUpdateProperties = MakeShareable(new FJsonObject);
	ServerUpdateProperties->SetStringField("current_server", Server);
	TDSHelper::JsonObjectAppend(ServerUpdateProperties, CustomOperationProperties(nullptr));
	TapDBEventUser->UpdateEvent(nullptr, ServerUpdateProperties, TauDBEnum::Normal);

}

void TauDBImplement::SetName(const FString& Name)
{
	if (!CheckIdentify() || !CheckStringParam(Name, "Name"))
	{
		return;
	}
	TSharedPtr<FJsonObject> NameProperties = MakeShareable(new FJsonObject);
	NameProperties->SetStringField("user_name", Name);
	TDSHelper::JsonObjectAppend(NameProperties, CustomOperationProperties(nullptr));
	TapDBEventUser->UpdateEvent(nullptr, NameProperties, TauDBEnum::Normal);
}

void TauDBImplement::ChargeSuccess(FString OrderId, FString Product, int64 Amount,
	FString CurrencyType, FString Payment, TSharedPtr<FJsonObject> Properties)
{
	if (!CheckIdentify()) {
		return;
	}
	if (Amount <= 0 || Amount > 100000000000) {
		TDUDebuger::ErrorLog("amount is illegal, shoud > 0 and <= 100000000000");
		return;
	}
	if (!CheckStringParam(OrderId, "OrderId")) {
		OrderId = "";
	}
	if (!CheckStringParam(Product, "Product")) {
		Product = "";
	}
	if (!CheckStringParam(CurrencyType, "CurrencyType")) {
		CurrencyType = "";
	}
	if (!CheckStringParam(Payment, "Payment")) {
		Payment = "";
	}
	TSharedPtr<FJsonObject> EventDataDic = MakeShareable(new FJsonObject);
	EventDataDic->SetStringField("order_id", OrderId);
	EventDataDic->SetStringField("product", Product);
	EventDataDic->SetStringField("payment", Payment);
	EventDataDic->SetStringField("currency_type", CurrencyType);
	EventDataDic->SetNumberField("amount", Amount);
	TapDBEventUser->TrackEvent("charge", Properties, EventDataDic);
}

void TauDBImplement::TrackEvent(const FString& EventName, TSharedPtr<FJsonObject> Properties)
{
	if (EventName.IsEmpty())
	{
		TDUDebuger::ErrorLog("eventName is empty, will do nothing.");
		return;
	}
	// 设备事件需要带上用户属性
	TSharedPtr<FJsonObject> SystemParams = MakeShareable(new FJsonObject);
	SystemParams->SetStringField(TapDBEventMobile->GetIdentifyKey(), TapDBEventUser->GetIdentify());
	SystemParams->SetStringField(TauDBEventKey::OPENID_KEY, TapDBEventUser->GetOpenId());
	TapDBEventMobile->TrackEvent(EventName, Properties, TapDBEventUser->CommonProperties, TauDBEnum::Normal, SystemParams);
}

void TauDBImplement::DeviceInitialize(TSharedPtr<FJsonObject> Properties)
{
	TapDBEventMobile->InitialEvent(nullptr, CustomOperationProperties(Properties), TauDBEnum::Normal);
}

void TauDBImplement::DeviceUpdate(TSharedPtr<FJsonObject> Properties)
{
	TapDBEventMobile->UpdateEvent(nullptr, CustomOperationProperties(Properties), TauDBEnum::Normal);
}

void TauDBImplement::DeviceAdd(TSharedPtr<FJsonObject> Properties)
{
	TapDBEventMobile->AddEvent(nullptr, CustomOperationProperties(Properties), TauDBEnum::Normal);
}

void TauDBImplement::UserInitialize(TSharedPtr<FJsonObject> Properties)
{
	TapDBEventUser->InitialEvent(nullptr, CustomOperationProperties(Properties), TauDBEnum::Normal);
}

void TauDBImplement::UserUpdate(TSharedPtr<FJsonObject> Properties)
{
	TapDBEventUser->UpdateEvent(nullptr, CustomOperationProperties(Properties), TauDBEnum::Normal);
}

void TauDBImplement::UserAdd(TSharedPtr<FJsonObject> Properties)
{
	TapDBEventUser->AddEvent(nullptr, CustomOperationProperties(Properties), TauDBEnum::Normal);
}

void TauDBImplement::RegisterStaticProperties(TSharedPtr<FJsonObject> StaticProperties)
{
	TapDBEventMobile->CustomStaticProperties = StaticProperties;
	TapDBEventUser->CustomStaticProperties = StaticProperties;
}

void TauDBImplement::ClearStaticProperties()
{
	RegisterStaticProperties(nullptr);
}

void TauDBImplement::UnregisterStaticProperty(const FString& PropertyName)
{
	if (TapDBEventMobile->CustomStaticProperties.IsValid())
	{
		TapDBEventMobile->CustomStaticProperties->RemoveField(PropertyName);

	}
	if (TapDBEventUser->CustomStaticProperties.IsValid())
	{
		TapDBEventUser->CustomStaticProperties->RemoveField(PropertyName);

	}
}

void TauDBImplement::RegisterDynamicProperties(TFunction<TSharedPtr<FJsonObject>()> DynamicPropertiesCaculator)
{
	TapDBEventMobile->CustomDynamicPropertiesCaculator = DynamicPropertiesCaculator;
	TapDBEventUser->CustomDynamicPropertiesCaculator = DynamicPropertiesCaculator;
}

TSharedPtr<FJsonObject> TauDBImplement::CustomOperationProperties(const TSharedPtr<FJsonObject>& Properties)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TDSHelper::JsonObjectAppend(JsonObject, Properties);
	JsonObject->SetStringField("sdk_version", TAPDB_VERSION);
	return JsonObject;
}

bool TauDBImplement::CheckIdentify()
{
	if (TapDBEventUser->GetIdentify().IsEmpty())
	{
		TDUDebuger::ErrorLog("please call setUser first");
		return false;
	}
	return true;
}

void TauDBImplement::SendPlayTime(int Interval)
{
	TSharedPtr<FJsonObject> TimeProperties = MakeShareable(new FJsonObject);
	TimeProperties->SetNumberField("duration", Interval);
	TapDBEventUser->TrackEvent("play_game", nullptr, TimeProperties);
}

bool TauDBImplement::CheckStringParam(const FString& Para, const FString& ParaName)
{
	if (Para.IsEmpty() || Para.Len() > 256)
	{
		TDUDebuger::ErrorLog(FString::Printf(TEXT("%s is illegal, length should > 0 and <= 256"), *ParaName));
		return false;
	}
	return true;
}

void TauDBImplement::RegisterCoreDelegate()
{
	FCoreDelegates::ApplicationWillDeactivateDelegate.AddLambda([=]()
	{
		TDUDebuger::DisplayLog("Enter background.");
		PauseTime = MakeShareable(new FDateTime(FDateTime::Now()));
	});

	FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddLambda([=]()
	{
		FDateTime Now = FDateTime::Now();
		if (StartTime.IsValid())
		{
			StartTime = MakeShareable(new FDateTime(Now));
			PauseTime = nullptr;
		} else if (PauseTime.IsValid())
		{
			if ((Now - *PauseTime.Get()).GetTotalSeconds() > 30)
			{  //算作一次新的游戏
				TDUDebuger::DisplayLog("Start with a new session.");
				auto PlayTime = (*PauseTime.Get() - *StartTime.Get()).GetTotalSeconds();
				if (PlayTime > 0)
				{
					SendPlayTime(PlayTime);
				}
				StartTime = MakeShareable(new FDateTime(Now));
				PauseTime = nullptr;
			} else
			{
				PauseTime = nullptr;
			}
			
		} 
	});

	FCoreDelegates::ApplicationWillTerminateDelegate.AddLambda([=]()
	{
		if (!StartTime.IsValid())
		{
			return;
		}
		FDateTime Now = FDateTime::Now();
		// 先把游戏事件缓存到本地，然后发送给服务端，如果发送成功，那么删除，不然下次激活的时候发送
		auto PlayTime = (Now - *StartTime.Get()).GetTotalSeconds();
		if (PlayTime > 0)
		{
			DataStorage<FTauDBStorage>::SaveNumber(FTauDBStorage::TapDBPlayTime, PlayTime);
			SendPlayTime(PlayTime);
			// 如果请求失败，等待2秒无限重试；
			static FTimerHandle TimerHandle;
			if (GWorld)
			{
				GWorld->GetWorld()->GetTimerManager().SetTimer(TimerHandle, [=]()
				{
					// 说明所有请求都发送成功了
					if (TAUDBNet::CacheCount == 0)
					{
						TDUDebuger::DisplayLog("All data sended before terminated.");
						DataStorage<FTauDBStorage>::Remove(FTauDBStorage::TapDBPlayTime);
						GWorld->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
					}
				}, 0.5, true);
			}
		}
	});
}

void TauDBImplement::StartOperation(const TSharedPtr<FJsonObject>& Properties)
{
	if (TapDBEventMobile->HasSavedIdentify())
	{
		TapDBEventMobile->AutoIdentifyWithProperties(Properties);
	} else
	{
		TapDBEventMobile->Identify(DeviceInfo::GetLoginId(), FString(), Properties);
	}
}

