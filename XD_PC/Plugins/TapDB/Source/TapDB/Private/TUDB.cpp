// Fill out your copyright notice in the Description page of Project Settings.


#include "TUDB.h"

#include "TUDeviceInfo.h"
#include "TUDBConfig.h"
#include "TUDBImplement.h"
#include "TUDebuger.h"

void UTUDB::StartByAppID(const TUDBInitConfig& Config)
{
	Start(false, Config);
}

void UTUDB::StartByClientID(const TUDBInitConfig& Config)
{
	Start(true, Config);
}

void UTUDB::SetUser(const FString& UserId, const FString& LoginType, TSharedPtr<FJsonObject> Properties)
{
	TUDBImplement::Get().SetUser(UserId, Properties, LoginType);
}

void UTUDB::ClearUser()
{
	TUDBImplement::Get().ClearUser();
}

TSharedPtr<FJsonObject> UTUDB::GetStartInfo()
{
	return TUDBImplement::Get().GetStartInfo();
}

FString UTUDB::GetDeviceId()
{
	return TUDBImplement::Get().GetDeviceId();
}

void UTUDB::SetLevel(int Level)
{
	TUDBImplement::Get().SetLevel(Level);
}

void UTUDB::SetServer(const FString& Server)
{
	TUDBImplement::Get().SetServer(Server);
}

void UTUDB::SetName(const FString& Name)
{
	TUDBImplement::Get().SetName(Name);
}

void UTUDB::ChargeSuccess(FString OrderId, FString Product, int64 Amount, FString CurrencyType, FString Payment, TSharedPtr<FJsonObject> Properties)
{
	TUDBImplement::Get().ChargeSuccess(OrderId, Product, Amount, CurrencyType, Payment, Properties);
}

void UTUDB::TrackEvent(const FString& EventName, TSharedPtr<FJsonObject> Properties)
{
	TUDBImplement::Get().TrackEvent(EventName, Properties);
}

void UTUDB::DeviceInitialize(TSharedPtr<FJsonObject> Properties)
{
	TUDBImplement::Get().DeviceInitialize(Properties);
}

void UTUDB::DeviceUpdate(TSharedPtr<FJsonObject> Properties)
{
	TUDBImplement::Get().DeviceUpdate(Properties);
}

void UTUDB::DeviceAdd(TSharedPtr<FJsonObject> Properties)
{
	TUDBImplement::Get().DeviceAdd(Properties);
}

void UTUDB::UserInitialize(TSharedPtr<FJsonObject> Properties)
{
	TUDBImplement::Get().UserInitialize(Properties);
}

void UTUDB::UserUpdate(TSharedPtr<FJsonObject> Properties)
{
	TUDBImplement::Get().UserUpdate(Properties);
}

void UTUDB::UserAdd(TSharedPtr<FJsonObject> Properties)
{
	TUDBImplement::Get().UserAdd(Properties);
}

void UTUDB::RegisterStaticProperties(TSharedPtr<FJsonObject> StaticProperties)
{
	TUDBImplement::Get().RegisterStaticProperties(StaticProperties);
}

void UTUDB::ClearStaticProperties()
{
	TUDBImplement::Get().ClearStaticProperties();
}

void UTUDB::UnregisterStaticProperty(const FString& PropertyName)
{
	TUDBImplement::Get().UnregisterStaticProperty(PropertyName);
}

void UTUDB::RegisterDynamicProperties(TFunction<TSharedPtr<FJsonObject>()> DynamicPropertiesCaculator)
{
	TUDBImplement::Get().RegisterDynamicProperties(DynamicPropertiesCaculator);
}

void UTUDB::Start(bool isClientID, const TUDBInitConfig& Config)
{
	if (Config.AppIdOrClientId.Len() <= 0 || Config.AppIdOrClientId.Len() > 256)
	{
		TUDebuger::ErrorLog("AppIdOrClientId is illegal, TapDB will not work.");
	}
	TUDBConfig::Get().isCN = Config.isCN;
	TUDBImplement::Get().Init(Config, !isClientID);
}
