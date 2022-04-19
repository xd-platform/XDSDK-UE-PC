// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InitConfigModel.generated.h"

USTRUCT()
struct FBindEntriesConfig
{
	GENERATED_BODY()
	
	UPROPERTY()
	int canBind;
	
	UPROPERTY()
	FString entryName;

	UPROPERTY()
	int canUnbind;
};

USTRUCT()
struct FTapSdkConfig
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString clientId;
	
	UPROPERTY()
	FString tapDBChannel;
	
	UPROPERTY()
	FString clientToken;
	
	UPROPERTY()
	FString serverUrl;

	UPROPERTY()
	bool enableTapDB;
};

USTRUCT()
struct FConfigsModel
{
	GENERATED_BODY()

	UPROPERTY()
	FString webPayUrl;

	UPROPERTY()
	FString serviceAgreementUrl;

	UPROPERTY()
	FString serviceAgreementTxt;

	UPROPERTY()
	FString googlePlayGamesAndroidClientId;

	UPROPERTY()
	FString serviceTermsUrl;

	UPROPERTY()
	FString serviceTermsTxt;

	UPROPERTY()
	FString reportUrl;

	UPROPERTY()
	TArray<FBindEntriesConfig> bindEntriesConfig;

	UPROPERTY()
	TArray<FString> androidLoginEntries;

	UPROPERTY()
	bool enableFirebaseTrack;

	UPROPERTY()
	TArray<FString> iosLoginEntries;

	UPROPERTY()
	FString californiaPrivacyUrl;

	UPROPERTY()
	FString gameName;

	UPROPERTY()
	FTapSdkConfig tapSdkConfig;

	UPROPERTY()
	TArray<FString> tapLoginPermissions;

	UPROPERTY()
	TArray<FString> bindEntries;

	UPROPERTY()
	FString appId;

	UPROPERTY()
	FString facebookClientId;

	UPROPERTY()
	FString region;

	UPROPERTY()
	bool isKRPushServiceSwitchEnable;
	
};

USTRUCT()
struct FInitConfigModel
{
	GENERATED_BODY()

	UPROPERTY()
	FString version;

	UPROPERTY()
	FString groupId;

	UPROPERTY()
	FConfigsModel configs;
	
};

