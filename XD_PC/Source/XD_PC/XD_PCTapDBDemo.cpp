// Fill out your copyright notice in the Description page of Project Settings.


#include "XD_PCTapDBDemo.h"

#include "TUDB.h"

void UXD_PCTapDBDemo::TapDBDemoReadme() {
	FPlatformProcess::LaunchURL(TEXT("https://confluence.xindong.com/pages/viewpage.action?pageId=669527742"), nullptr, nullptr);
}

void UXD_PCTapDBDemo::Init(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	TUDBInitConfig Config;
	Config.AppIdOrClientId = Paras[0];
	Config.Channel = Paras[1];
	Config.GameVersion = Paras[2];
	Config.isCN = Paras[3].ToBool();
	Config.Properties = GetProperties(Properties);
	UTUDB::StartByClientID(Config);
}

void UXD_PCTapDBDemo::SetUser(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	UTUDB::SetUser(Paras[0], Paras[1], GetProperties(Properties));
}

void UXD_PCTapDBDemo::ClearUser(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	UTUDB::ClearUser();
}

void UXD_PCTapDBDemo::SetLevel(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	UTUDB::SetLevel(FCString::Atoi(*Paras[0]));
}

void UXD_PCTapDBDemo::SetServer(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	UTUDB::SetServer(Paras[0]);
}

void UXD_PCTapDBDemo::SetName(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	UTUDB::SetName(Paras[0]);
}

void UXD_PCTapDBDemo::ChargeSuccess(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	UTUDB::ChargeSuccess(Paras[0], Paras[1], FCString::Atoi(*Paras[2]), Paras[3], Paras[4], GetProperties(Properties));
	// void UTauDB::ChargeSuccess(FString OrderId, FString Product, int64 Amount, FString CurrencyType, FString Payment, TSharedPtr<FJsonObject> Properties)

}

void UXD_PCTapDBDemo::TrackEvent(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	UTUDB::TrackEvent(Paras[0], GetProperties(Properties));
}

void UXD_PCTapDBDemo::DeviceInitialize(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	UTUDB::DeviceInitialize(GetProperties(Properties));
}

void UXD_PCTapDBDemo::DeviceUpdate(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	UTUDB::DeviceUpdate(GetProperties(Properties));
}

void UXD_PCTapDBDemo::DeviceAdd(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	UTUDB::DeviceAdd(GetProperties(Properties));
}

void UXD_PCTapDBDemo::UserInitialize(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	UTUDB::UserInitialize(GetProperties(Properties));
}

void UXD_PCTapDBDemo::UserUpdate(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	UTUDB::UserUpdate(GetProperties(Properties));
}

void UXD_PCTapDBDemo::UserAdd(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	UTUDB::UserAdd(GetProperties(Properties));
}

void UXD_PCTapDBDemo::RegisterStaticProperties(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	UTUDB::RegisterStaticProperties(GetProperties(Properties));
}

void UXD_PCTapDBDemo::ClearStaticProperties(const TArray<FString>& Paras, const TMap<FString, FString> Properties)
{
	UTUDB::ClearStaticProperties();
}
