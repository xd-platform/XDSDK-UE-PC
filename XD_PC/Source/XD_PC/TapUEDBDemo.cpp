// Fill out your copyright notice in the Description page of Project Settings.


#include "TapUEDBDemo.h"

#include "TapUEDB.h"

void UTapUEDBDemo::TapDBDemoReadme() {
	FPlatformProcess::LaunchURL(TEXT("https://confluence.xindong.com/pages/viewpage.action?pageId=669527742"), nullptr, nullptr);
}

void UTapUEDBDemo::Init(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	TUDBType::Config Config;
	Config.ClientId = Paras[0];
	Config.Channel = Paras[1];
	Config.GameVersion = Paras[2];
	Config.RegionType = (TUType::RegionType)FCString::Atoi(*Paras[3]);
	TapUEDB::Init(Config);
}

void UTapUEDBDemo::SetUser(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	TapUEDB::SetUserWithLoginType(Paras[0], Paras[1]);
}

void UTapUEDBDemo::ClearUser(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	TapUEDB::ClearUser();
}

void UTapUEDBDemo::SetLevel(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	TapUEDB::SetLevel(FCString::Atoi(*Paras[0]));
}

void UTapUEDBDemo::SetServer(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	TapUEDB::SetServer(Paras[0]);
}

void UTapUEDBDemo::SetName(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	TapUEDB::SetName(Paras[0]);
}

void UTapUEDBDemo::ChargeSuccess(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	TapUEDB::OnCharge(Paras[0], Paras[1], FCString::Atoi(*Paras[2]), Paras[3], Paras[4], GetProperties(Properties));
	// void UTauDB::ChargeSuccess(FString OrderId, FString Product, int64 Amount, FString CurrencyType, FString Payment, TSharedPtr<FJsonObject> Properties)

}

void UTapUEDBDemo::TrackEvent(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	TapUEDB::TrackEvent(Paras[0], GetProperties(Properties));
}

void UTapUEDBDemo::DeviceInitialize(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	TapUEDB::DeviceInitialize(GetProperties(Properties));
}

void UTapUEDBDemo::DeviceUpdate(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	TapUEDB::DeviceUpdate(GetProperties(Properties));
}

void UTapUEDBDemo::DeviceAdd(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	TapUEDB::DeviceAdd(GetProperties(Properties));
}

void UTapUEDBDemo::UserInitialize(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	TapUEDB::UserInitialize(GetProperties(Properties));
}

void UTapUEDBDemo::UserUpdate(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	TapUEDB::UserUpdate(GetProperties(Properties));
}

void UTapUEDBDemo::UserAdd(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	TapUEDB::UserAdd(GetProperties(Properties));
}

void UTapUEDBDemo::RegisterStaticProperties(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	TapUEDB::RegisterStaticProperties(GetProperties(Properties));
}

void UTapUEDBDemo::ClearStaticProperties(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	TapUEDB::ClearStaticProperties();
}