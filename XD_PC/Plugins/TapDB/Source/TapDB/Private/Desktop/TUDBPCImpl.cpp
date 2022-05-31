#include "TUDBPCImpl.h"
#include "TUDBNet.h"
#include "TUDBStorage.h"
#include "TUDeviceInfo.h"
#include "TUHelper.h"

FString TUDBPCImpl::First_login_type = "";
FString TUDBPCImpl::First_open_id = "";
FString TUDBPCImpl::TAPDB_VERSION = "3.0.9";

void TUDBPCImpl::Init(TUDBType::Config InitConfig) {
	Config = InitConfig;
	TapDBEventMobile = MakeShareable(new TUDBEventMobile(Config.ClientId));
	TapDBEventUser = MakeShareable(new TUDBEventUser(Config.ClientId));

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
	TapDBEventUser->CommonProperties = CommonProperties;
	
	StartTime = MakeShareable(new FDateTime(FDateTime::Now()));
	RegisterCoreDelegate();
	
	StartOperation(nullptr);
	
	TSharedPtr<FJsonObject> DeviceIdProperty = MakeShareable(new FJsonObject);
	DeviceIdProperty->SetStringField(TapDBEventMobile->GetIdentifyKey(), TapDBEventMobile->GetIdentify());
	TapDBEventUser->SysProperties = DeviceIdProperty;
}

void TUDBPCImpl::SetUserWithLoginType(const FString& UserId, const FString& LoginType) {
	if (!CheckStringParam(UserId, "UserId"))
	{
		return;
	}
	TapDBEventUser->Identify(UserId, LoginType, nullptr);

	TSharedPtr<FJsonObject> InitialOperationProperties = MakeShareable(new FJsonObject);
	if (First_login_type.IsEmpty())
	{
		First_login_type = LoginType;
		InitialOperationProperties->SetStringField("first_login_type", LoginType);
	}

	TSharedPtr<FJsonObject> UpdateOperationProperties = MakeShareable(new FJsonObject);
	UpdateOperationProperties->SetBoolField("has_user", true);
	UpdateOperationProperties->SetStringField("current_login_type", LoginType);

	if (LoginType == TUDBType::LoginType::TapTap)
	{
		if (First_open_id.IsEmpty())
		{
			FString OpenId = TapDBEventUser->GetOpenId();
			First_open_id = OpenId;
			InitialOperationProperties->SetStringField("first_open_id", OpenId);
		}
	}

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

void TUDBPCImpl::ClearUser() {
	TapDBEventUser->ClearIdentify();
}

void TUDBPCImpl::SetName(const FString& Name) {
	if (!CheckIdentify() || !CheckStringParam(Name, "Name"))
	{
		return;
	}
	TSharedPtr<FJsonObject> NameProperties = MakeShareable(new FJsonObject);
	NameProperties->SetStringField("user_name", Name);
	TUHelper::JsonObjectAppend(NameProperties, CustomOperationProperties(nullptr));
	TapDBEventUser->UpdateEvent(nullptr, NameProperties, TUDBEnum::Normal);
}

void TUDBPCImpl::SetLevel(int Level) {
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

void TUDBPCImpl::SetServer(const FString& Server) {
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

void TUDBPCImpl::OnCharge(const FString& OrderId, const FString& Product, int Amount, const FString& CurrencyType,
	const FString& Payment, TSharedPtr<FJsonObject> Properties) {
	if (!CheckIdentify()) {
		return;
	}
	if (Amount <= 0 || Amount > 100000000000) {
		TUDebuger::ErrorLog("amount is illegal, shoud > 0 and <= 100000000000");
		return;
	}
	TSharedPtr<FJsonObject> EventDataDic = MakeShareable(new FJsonObject);
	EventDataDic->SetStringField("order_id", CheckStringParam(OrderId, "OrderId") ? OrderId : "");
	EventDataDic->SetStringField("product", CheckStringParam(Product, "Product") ? Product : "");
	EventDataDic->SetStringField("payment", CheckStringParam(Payment, "Payment") ? Payment : "");
	EventDataDic->SetStringField("currency_type", CheckStringParam(CurrencyType, "CurrencyType") ? CurrencyType : "");
	EventDataDic->SetNumberField("amount", Amount);
	TapDBEventUser->TrackEvent("charge", Properties, EventDataDic);
}

void TUDBPCImpl::TrackEvent(const FString& EventName, TSharedPtr<FJsonObject> Properties) {
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

void TUDBPCImpl::RegisterStaticProperties(TSharedPtr<FJsonObject> Properties) {
	TapDBEventMobile->CustomStaticProperties = Properties;
	TapDBEventUser->CustomStaticProperties = Properties;
}

void TUDBPCImpl::UnregisterStaticProperty(const FString& Key) {
	if (TapDBEventMobile->CustomStaticProperties.IsValid())
	{
		TapDBEventMobile->CustomStaticProperties->RemoveField(Key);

	}
	if (TapDBEventUser->CustomStaticProperties.IsValid())
	{
		TapDBEventUser->CustomStaticProperties->RemoveField(Key);

	}
}

void TUDBPCImpl::ClearStaticProperties() {
	RegisterStaticProperties(nullptr);
}

void TUDBPCImpl::RegisterDynamicProperties(TFunction<TSharedPtr<FJsonObject>()> PropertiesBlock) {
	TapDBEventMobile->CustomDynamicPropertiesCaculator = PropertiesBlock;
	TapDBEventUser->CustomDynamicPropertiesCaculator = PropertiesBlock;
}

void TUDBPCImpl::DeviceInitialize(TSharedPtr<FJsonObject> Properties) {
	TapDBEventMobile->InitialEvent(nullptr, CustomOperationProperties(Properties), TUDBEnum::Normal);
}

void TUDBPCImpl::DeviceUpdate(TSharedPtr<FJsonObject> Properties) {
	TapDBEventMobile->UpdateEvent(nullptr, CustomOperationProperties(Properties), TUDBEnum::Normal);
}

void TUDBPCImpl::DeviceAdd(TSharedPtr<FJsonObject> Properties) {
	TapDBEventMobile->AddEvent(nullptr, CustomOperationProperties(Properties), TUDBEnum::Normal);
}

void TUDBPCImpl::UserInitialize(TSharedPtr<FJsonObject> Properties) {
	TapDBEventUser->InitialEvent(nullptr, CustomOperationProperties(Properties), TUDBEnum::Normal);
}

void TUDBPCImpl::UserUpdate(TSharedPtr<FJsonObject> Properties) {
	TapDBEventUser->UpdateEvent(nullptr, CustomOperationProperties(Properties), TUDBEnum::Normal);
}

void TUDBPCImpl::UserAdd(TSharedPtr<FJsonObject> Properties) {
	TapDBEventUser->AddEvent(nullptr, CustomOperationProperties(Properties), TUDBEnum::Normal);
}

TSharedPtr<FJsonObject> TUDBPCImpl::CustomOperationProperties(const TSharedPtr<FJsonObject>& Properties) {
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TUHelper::JsonObjectAppend(JsonObject, Properties);
	JsonObject->SetStringField("sdk_version", TAPDB_VERSION);
	return JsonObject;
}

bool TUDBPCImpl::CheckIdentify() {
	if (TapDBEventUser->GetIdentify().IsEmpty())
	{
		TUDebuger::ErrorLog("please call setUser first");
		return false;
	}
	return true;
}

void TUDBPCImpl::SendPlayTime(int Interval) {
	TSharedPtr<FJsonObject> TimeProperties = MakeShareable(new FJsonObject);
	TimeProperties->SetNumberField("duration", Interval);
	TapDBEventUser->TrackEvent("play_game", nullptr, TimeProperties);
}

bool TUDBPCImpl::CheckStringParam(const FString& Para, const FString& ParaName) {
	if (Para.IsEmpty() || Para.Len() > 256)
	{
		TUDebuger::ErrorLog(FString::Printf(TEXT("%s is illegal, length should > 0 and <= 256"), *ParaName));
		return false;
	}
	return true;
}

void TUDBPCImpl::RegisterCoreDelegate() {
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

void TUDBPCImpl::StartOperation(const TSharedPtr<FJsonObject>& Properties) {
	if (TapDBEventMobile->HasSavedIdentify())
	{
		TapDBEventMobile->AutoIdentifyWithProperties(Properties);
	} else
	{
		TapDBEventMobile->Identify(TUDeviceInfo::GetLoginId(), FString(), Properties);
	}
}


