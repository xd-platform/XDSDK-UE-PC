// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TokenModel.h"
#include "XDGEnumType.h"
#include "XDGUser.generated.h"

USTRUCT()
struct FXDGUser
{
	GENERATED_BODY()

	UPROPERTY()
	FString userId;
	
	UPROPERTY()
	FString username;

	UPROPERTY()
	FString nickName;

	UPROPERTY()
	uint64 loginType;

	UPROPERTY()
	TArray<FString> loginList; // boundAccounts

	TSharedPtr<FTokenModel> Token()
	{
		return  FTokenModel::GetCurrentToken();
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


