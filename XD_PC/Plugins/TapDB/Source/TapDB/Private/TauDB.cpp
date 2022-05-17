// Fill out your copyright notice in the Description page of Project Settings.


#include "TauDB.h"

#include "TUDeviceInfo.h"
#include "TauDBConfig.h"
#include "TauDBImplement.h"
#include "TUDebuger.h"

void UTauDB::StartByAppID(const TauDBInitConfig& Config)
{
	Start(false, Config);
}

void UTauDB::StartByClientID(const TauDBInitConfig& Config)
{
	Start(true, Config);
}

void UTauDB::SetUser(const FString& UserId, const FString& LoginType, TSharedPtr<FJsonObject> Properties)
{
	TauDBImplement::Get().SetUser(UserId, Properties, LoginType);
}

void UTauDB::ClearUser()
{
	TauDBImplement::Get().ClearUser();
}

TSharedPtr<FJsonObject> UTauDB::GetStartInfo()
{
	return TauDBImplement::Get().GetStartInfo();
}

FString UTauDB::GetDeviceId()
{
	return TauDBImplement::Get().GetDeviceId();
}

void UTauDB::SetLevel(int Level)
{
	TauDBImplement::Get().SetLevel(Level);
}

void UTauDB::SetServer(const FString& Server)
{
	TauDBImplement::Get().SetServer(Server);
}

void UTauDB::SetName(const FString& Name)
{
	TauDBImplement::Get().SetName(Name);
}

void UTauDB::ChargeSuccess(FString OrderId, FString Product, int64 Amount, FString CurrencyType, FString Payment, TSharedPtr<FJsonObject> Properties)
{
	TauDBImplement::Get().ChargeSuccess(OrderId, Product, Amount, CurrencyType, Payment, Properties);
}

void UTauDB::TrackEvent(const FString& EventName, TSharedPtr<FJsonObject> Properties)
{
	TauDBImplement::Get().TrackEvent(EventName, Properties);
}

void UTauDB::DeviceInitialize(TSharedPtr<FJsonObject> Properties)
{
	TauDBImplement::Get().DeviceInitialize(Properties);
}

void UTauDB::DeviceUpdate(TSharedPtr<FJsonObject> Properties)
{
	TauDBImplement::Get().DeviceUpdate(Properties);
}

void UTauDB::DeviceAdd(TSharedPtr<FJsonObject> Properties)
{
	TauDBImplement::Get().DeviceAdd(Properties);
}

void UTauDB::UserInitialize(TSharedPtr<FJsonObject> Properties)
{
	TauDBImplement::Get().UserInitialize(Properties);
}

void UTauDB::UserUpdate(TSharedPtr<FJsonObject> Properties)
{
	TauDBImplement::Get().UserUpdate(Properties);
}

void UTauDB::UserAdd(TSharedPtr<FJsonObject> Properties)
{
	TauDBImplement::Get().UserAdd(Properties);
}

void UTauDB::RegisterStaticProperties(TSharedPtr<FJsonObject> StaticProperties)
{
	TauDBImplement::Get().RegisterStaticProperties(StaticProperties);
}

void UTauDB::ClearStaticProperties()
{
	TauDBImplement::Get().ClearStaticProperties();
}

void UTauDB::UnregisterStaticProperty(const FString& PropertyName)
{
	TauDBImplement::Get().UnregisterStaticProperty(PropertyName);
}

void UTauDB::RegisterDynamicProperties(TFunction<TSharedPtr<FJsonObject>()> DynamicPropertiesCaculator)
{
	TauDBImplement::Get().RegisterDynamicProperties(DynamicPropertiesCaculator);
}

void UTauDB::Start(bool isClientID, const TauDBInitConfig& Config)
{
	if (Config.AppIdOrClientId.Len() <= 0 || Config.AppIdOrClientId.Len() > 256)
	{
		TUDebuger::ErrorLog("AppIdOrClientId is illegal, TapDB will not work.");
	}
	TauDBConfig::Get().isCN = Config.isCN;
	TauDBImplement::Get().Init(Config, !isClientID);
}
