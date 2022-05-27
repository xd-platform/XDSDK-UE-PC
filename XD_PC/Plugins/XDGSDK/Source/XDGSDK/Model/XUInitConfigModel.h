// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XUStorage.h"
#include "XUInitConfigModel.generated.h"

USTRUCT()
struct FXUBindEntriesConfig
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
struct FXUTapSdkConfig
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
struct FXUConfigsModel
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
	TArray<FXUBindEntriesConfig> bindEntriesConfig;

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
	FXUTapSdkConfig tapSdkConfig;

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
struct FXUInitConfigModel
{
	GENERATED_BODY()

	UPROPERTY()
	FString version;

	UPROPERTY()
	FString groupId;

	UPROPERTY()
	FXUConfigsModel configs;

	void SaveToLocal();

	static TSharedPtr<FXUInitConfigModel>& GetLocalModel();

	static bool CanShowPrivacyAlert();

	static void UpdatePrivacyState(); //弹过之后若没变化就不再弹出
	
	static void GetPrivacyTxt(const FString& txtUrl, TFunction<void(FString txt)> callback);

private:
	static TSharedPtr<FXUInitConfigModel> CurrentModel;
	void SavePrivacyTxt();

	TMap<FString, FString> Cache;
};

