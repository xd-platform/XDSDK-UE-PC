// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XUStorage.h"
#include "XUType.h"
#include "XULocalInitConfigModel.generated.h"

USTRUCT()
struct FXULocalTapDBConfig
{
	GENERATED_BODY()
	
	UPROPERTY()
	bool enable;
	
	UPROPERTY()
	FString channel;

	UPROPERTY()
	FString game_version;
};

USTRUCT()
struct FXULocalTapSDKConfig
{
	GENERATED_BODY()

	UPROPERTY()
	FString client_id;

	UPROPERTY()
	FString client_token;

	UPROPERTY()
	FString server_url;

	UPROPERTY()
	FXULocalTapDBConfig db_config;

	UPROPERTY()
	TArray<FString> permissions;
};

USTRUCT()
struct FXULocalFacebookConfig
{
	GENERATED_BODY()

	UPROPERTY()
	FString app_id;

	UPROPERTY()
	FString client_token;

	UPROPERTY()
	TArray<FString> permissions;
};

USTRUCT()
struct FXULocalLineConfig
{
	GENERATED_BODY()

	UPROPERTY()
	FString channel_id;

};

USTRUCT()
struct FXULocalTwitterConfig
{
	GENERATED_BODY()

	UPROPERTY()
	FString consumer_key;

	UPROPERTY()
	FString consumer_secret;
	
};

USTRUCT()
struct FXULocalGoogleConfig
{
	GENERATED_BODY()

	UPROPERTY()
	FString CLIENT_ID;

	UPROPERTY()
	FString REVERSED_CLIENT_ID;

	UPROPERTY()
	FString API_KEY;

	UPROPERTY()
	FString GCM_SENDER_ID;

	UPROPERTY()
	int PLIST_VERSION;

	UPROPERTY()
	FString BUNDLE_ID;

	UPROPERTY()
	FString PROJECT_ID;

	UPROPERTY()
	FString STORAGE_BUCKET;

	UPROPERTY()
	bool IS_ADS_ENABLED;

	UPROPERTY()
	bool IS_ANALYTICS_ENABLED;

	UPROPERTY()
	bool IS_APPINVITE_ENABLED;

	UPROPERTY()
	bool IS_GCM_ENABLED;

	UPROPERTY()
	bool IS_SIGNIN_ENABLED;

	UPROPERTY()
	FString GOOGLE_APP_ID;

	UPROPERTY()
	FString DATABASE_URL;
};

USTRUCT()
struct FXULocalAdjustEvent
{
	GENERATED_BODY()

	UPROPERTY()
	FString event_name;

	UPROPERTY()
	FString token;
};


USTRUCT()
struct FXULocalAdjustConfig
{
	GENERATED_BODY()

	UPROPERTY()
	FString app_token;

	UPROPERTY()
	TArray<FXULocalAdjustEvent> events;
	
};

USTRUCT()
struct FXULocalAppsflyerConfig
{
	GENERATED_BODY()

	UPROPERTY()
	FString app_id;

	UPROPERTY()
	FString dev_key;
	
};


USTRUCT()
struct FXULocalInitConfig
{
	GENERATED_BODY()

	UPROPERTY()
	FString region_type;

	UPROPERTY()
	FString bundle_id;

	UPROPERTY()
	FString client_id;

	UPROPERTY()
	FString app_id;

	UPROPERTY()
	bool idfa_enabled;

	UPROPERTY()
	FString game_name;

	UPROPERTY()
	FString report_url;

	UPROPERTY()
	FString logout_url;

	UPROPERTY()
	FString webpay_url;

	UPROPERTY()
	FXULocalTapSDKConfig tapsdk;

	UPROPERTY()
	TArray<FString> logos;

	UPROPERTY()
	FXULocalFacebookConfig facebook;

	UPROPERTY()
	FXULocalLineConfig line;

	UPROPERTY()
	FXULocalTwitterConfig twitter;

	UPROPERTY()
	FXULocalGoogleConfig google;

	UPROPERTY()
	FXULocalAdjustConfig adjust;

	UPROPERTY()
	FXULocalAppsflyerConfig appsflyer;

	void SaveToLocal();

	static TSharedPtr<FXULocalInitConfig> GetLocalModel();

	TSharedPtr<XUType::Config> GenerateXUConfig();
	// static void SetLocalConfigJsonName(const FString& ConfigName);
};



