// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TokenModel.h"
#include "XDGEnumType.h"
#include "XDGUser.generated.h"

USTRUCT(BlueprintType)
struct FXDGUser
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString userId;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString username;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString nickName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int loginType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FString> loginList; // boundAccounts

	void SaveToLocal()
	{
		DataStorage::SaveStruct(XDGDataStorageName::UserInfo, *this, true);
	}

	static TSharedPtr<FXDGUser> GetLocalModel()
	{
		return  DataStorage::LoadStruct<FXDGUser>(XDGDataStorageName::UserInfo);
	}

	TSharedPtr<FTokenModel> Token()
	{
		return  FTokenModel::GetLocalModel();
	}

	LoginType GetLoginType()
	{
		if (loginType == 0){
			return LoginType::Guest;
		} else if (loginType == 5){
			return LoginType::TapTap;
		}
		return LoginType::Guest;
	}
};


