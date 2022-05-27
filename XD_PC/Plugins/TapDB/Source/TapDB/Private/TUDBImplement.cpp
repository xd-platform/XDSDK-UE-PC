#include "TUDBImplement.h"

#include "TUDeviceInfo.h"
#include "TUJsonHelper.h"
#include "TUDB.h"
#include "TUDBNet.h"
#include "TUDBStorage.h"
#include "TUHelper.h"

TUDBImplement * TUDBImplement::Singleton = nullptr;
FString TUDBImplement::First_login_type = "";
FString TUDBImplement::First_open_id = "";
FString TUDBImplement::TAPDB_VERSION = "3.0.9";


TUDBImplement & TUDBImplement::Get()
{
	if (Singleton == nullptr)
	{
		Singleton = new TUDBImplement;
	}
	return *Singleton;
}

void TUDBImplement::Init(const TUDBInitConfig& Config, bool isAccount)
{
	TapDBEventMobile = MakeShareable(new TUDBEventMobile(Config.AppIdOrClientId, isAccount));
	TapDBEventUser = MakeShareable(new TUDBEventUser(Config.AppIdOrClientId, isAccount));

	TSharedPtr<FJsonObject> CommonProperties = MakeShareable(new FJsonObject);
	if (Config.Channel.Len() > 0 && Config.Channel.Len() <= 256)
	{
		CommonProperties->SetStringField("channel", Config.Channel);
	}
	if (Config.GameVersion.Len() > 0 && Config.GameVersion.Len() <= 256)
	{
		CommonProperties->SetStringField("app_version", Config.GameVersion);
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

void TUDBImplement::SetUser(const FString& UserId, TSharedPtr<FJsonObject> Properties, const FString& LoginType)
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

	if (LoginType == TUDBLoginType::TapTap)
	{
		if (First_open_id.IsEmpty())
		{
			FString OpenId = TapDBEventUser->GetOpenId();
			First_open_id = OpenId;
			InitialOperationProperties->SetStringField("first_open_id", OpenId);
		}
	}

	TUHelper::JsonObjectAppend(InitialOperationProperties, Properties);
	
	TUHelper::JsonObjectAppend(UpdateOperationProperties, Properties);
	TUHelper::JsonObjectAppend(UpdateOperationProperties, CustomOperationProperties(nullptr));

	if (InitialOperationProperties->Values.Num() > 0)
	{
		TUHelper::JsonObjectAppend(InitialOperationProperties, CustomOperationProperties(nullptr));
		TapDBEventMobile->InitialEvent(nullptr, InitialOperationProperties, TUDBEnum::Normal);
	}
	TapDBEventMobile->UpdateEvent(nullptr, UpdateOperationProperties, TUDBEnum::Normal);

	double PlayTime = TUDataStorage<FTUDBStorage>::LoadNumber(FTUDBStorage::TapDBPlayTime);
	if (PlayTime > 0)
	{
		SendPlayTime(PlayTime);
		TUDataStorage<FTUDBStorage>::Remove(FTUDBStorage::TapDBPlayTime);
	}
	
}

void TUDBImplement::ClearUser()
{
	TapDBEventUser->ClearIdentify();
}

TSharedPtr<FJsonObject> TUDBImplement::GetStartInfo()
{
	return TapDBEventMobile->CommonProperties;
}

FString TUDBImplement::GetDeviceId()
{
	return TapDBEventMobile->GetIdentify();
}

void TUDBImplement::SetLevel(int Level)
{
	if (!CheckIdentify())
	{
		return;
	}
	if (Level < 0)
	{
		TUDebuger::ErrorLog("level is illegal, should >= 0");
		return;
	}
	TSharedPtr<FJsonObject> LevelProperties = MakeShareable(new FJsonObject);
	LevelProperties->SetNumberField("level", Level);
	TUHelper::JsonObjectAppend(LevelProperties, CustomOperationProperties(nullptr));
	TapDBEventUser->UpdateEvent(nullptr, LevelProperties, TUDBEnum::Normal);
}

void TUDBImplement::SetServer(const FString& Server)
{
	if (!CheckIdentify() || !CheckStringParam(Server, "Server"))
	{
		return;
	}
	
	TSharedPtr<FJsonObject> ServerInitialiseProperties = MakeShareable(new FJsonObject);
	ServerInitialiseProperties->SetStringField("first_server", Server);
	TUHelper::JsonObjectAppend(ServerInitialiseProperties, CustomOperationProperties(nullptr));
	TapDBEventUser->InitialEvent(nullptr, ServerInitialiseProperties, TUDBEnum::Normal);

	TSharedPtr<FJsonObject> ServerUpdateProperties = MakeShareable(new FJsonObject);
	ServerUpdateProperties->SetStringField("current_server", Server);
	TUHelper::JsonObjectAppend(ServerUpdateProperties, CustomOperationProperties(nullptr));
	TapDBEventUser->UpdateEvent(nullptr, ServerUpdateProperties, TUDBEnum::Normal);

}

void TUDBImplement::SetName(const FString& Name)
{
	if (!CheckIdentify() || !CheckStringParam(Name, "Name"))
	{
		return;
	}
	TSharedPtr<FJsonObject> NameProperties = MakeShareable(new FJsonObject);
	NameProperties->SetStringField("user_name", Name);
	TUHelper::JsonObjectAppend(NameProperties, CustomOperationProperties(nullptr));
	TapDBEventUser->UpdateEvent(nullptr, NameProperties, TUDBEnum::Normal);
}

void TUDBImplement::ChargeSuccess(FString OrderId, FString Product, int64 Amount,
	FString CurrencyType, FString Payment, TSharedPtr<FJsonObject> Properties)
{
	if (!CheckIdentify()) {
		return;
	}
	if (Amount <= 0 || Amount > 100000000000) {
		TUDebuger::ErrorLog("amount is illegal, shoud > 0 and <= 100000000000");
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

void TUDBImplement::TrackEvent(const FString& EventName, TSharedPtr<FJsonObject> Properties)
{
	if (EventName.IsEmpty())
	{
		TUDebuger::ErrorLog("eventName is empty, will do nothing.");
		return;
	}
	// 设备事件需要带上用户属性
	TSharedPtr<FJsonObject> SystemParams = MakeShareable(new FJsonObject);
	SystemParams->SetStringField(TapDBEventMobile->GetIdentifyKey(), TapDBEventUser->GetIdentify());
	SystemParams->SetStringField(TUDBEventKey::OPENID_KEY, TapDBEventUser->GetOpenId());
	TapDBEventMobile->TrackEvent(EventName, Properties, TapDBEventUser->CommonProperties, TUDBEnum::Normal, SystemParams);
}

void TUDBImplement::DeviceInitialize(TSharedPtr<FJsonObject> Properties)
{
	TapDBEventMobile->InitialEvent(nullptr, CustomOperationProperties(Properties), TUDBEnum::Normal);
}

void TUDBImplement::DeviceUpdate(TSharedPtr<FJsonObject> Properties)
{
	TapDBEventMobile->UpdateEvent(nullptr, CustomOperationProperties(Properties), TUDBEnum::Normal);
}

void TUDBImplement::DeviceAdd(TSharedPtr<FJsonObject> Properties)
{
	TapDBEventMobile->AddEvent(nullptr, CustomOperationProperties(Properties), TUDBEnum::Normal);
}

void TUDBImplement::UserInitialize(TSharedPtr<FJsonObject> Properties)
{
	TapDBEventUser->InitialEvent(nullptr, CustomOperationProperties(Properties), TUDBEnum::Normal);
}

void TUDBImplement::UserUpdate(TSharedPtr<FJsonObject> Properties)
{
	TapDBEventUser->UpdateEvent(nullptr, CustomOperationProperties(Properties), TUDBEnum::Normal);
}

void TUDBImplement::UserAdd(TSharedPtr<FJsonObject> Properties)
{
	TapDBEventUser->AddEvent(nullptr, CustomOperationProperties(Properties), TUDBEnum::Normal);
}

void TUDBImplement::RegisterStaticProperties(TSharedPtr<FJsonObject> StaticProperties)
{
	TapDBEventMobile->CustomStaticProperties = StaticProperties;
	TapDBEventUser->CustomStaticProperties = StaticProperties;
}

void TUDBImplement::ClearStaticProperties()
{
	RegisterStaticProperties(nullptr);
}

void TUDBImplement::UnregisterStaticProperty(const FString& PropertyName)
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

void TUDBImplement::RegisterDynamicProperties(TFunction<TSharedPtr<FJsonObject>()> DynamicPropertiesCaculator)
{
	TapDBEventMobile->CustomDynamicPropertiesCaculator = DynamicPropertiesCaculator;
	TapDBEventUser->CustomDynamicPropertiesCaculator = DynamicPropertiesCaculator;
}

TSharedPtr<FJsonObject> TUDBImplement::CustomOperationProperties(const TSharedPtr<FJsonObject>& Properties)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TUHelper::JsonObjectAppend(JsonObject, Properties);
	JsonObject->SetStringField("sdk_version", TAPDB_VERSION);
	return JsonObject;
}

bool TUDBImplement::CheckIdentify()
{
	if (TapDBEventUser->GetIdentify().IsEmpty())
	{
		TUDebuger::ErrorLog("please call setUser first");
		return false;
	}
	return true;
}

void TUDBImplement::SendPlayTime(int Interval)
{
	TSharedPtr<FJsonObject> TimeProperties = MakeShareable(new FJsonObject);
	TimeProperties->SetNumberField("duration", Interval);
	TapDBEventUser->TrackEvent("play_game", nullptr, TimeProperties);
}

bool TUDBImplement::CheckStringParam(const FString& Para, const FString& ParaName)
{
	if (Para.IsEmpty() || Para.Len() > 256)
	{
		TUDebuger::ErrorLog(FString::Printf(TEXT("%s is illegal, length should > 0 and <= 256"), *ParaName));
		return false;
	}
	return true;
}

void TUDBImplement::RegisterCoreDelegate()
{
	FCoreDelegates::ApplicationWillDeactivateDelegate.AddLambda([=]()
	{
		TUDebuger::DisplayLog("Enter background.");
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
				TUDebuger::DisplayLog("Start with a new session.");
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
			TUDataStorage<FTUDBStorage>::SaveNumber(FTUDBStorage::TapDBPlayTime, PlayTime);
			SendPlayTime(PlayTime);
			// 如果请求失败，等待2秒无限重试；
			static FTimerHandle TimerHandle;
			if (GWorld)
			{
				GWorld->GetWorld()->GetTimerManager().SetTimer(TimerHandle, [=]()
				{
					// 说明所有请求都发送成功了
					if (TUDBNet::CacheCount == 0)
					{
						TUDebuger::DisplayLog("All data sended before terminated.");
						TUDataStorage<FTUDBStorage>::Remove(FTUDBStorage::TapDBPlayTime);
						GWorld->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
					}
				}, 0.5, true);
			}
		}
	});
}

void TUDBImplement::StartOperation(const TSharedPtr<FJsonObject>& Properties)
{
	if (TapDBEventMobile->HasSavedIdentify())
	{
		TapDBEventMobile->AutoIdentifyWithProperties(Properties);
	} else
	{
		TapDBEventMobile->Identify(TUDeviceInfo::GetLoginId(), FString(), Properties);
	}
}

