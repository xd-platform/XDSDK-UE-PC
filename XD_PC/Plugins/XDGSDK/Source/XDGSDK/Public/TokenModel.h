// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XDGDataStorageName.h"
#include "TokenModel.generated.h"


USTRUCT()
struct FTokenModel
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString kid;

	UPROPERTY()
	FString macKey;

	UPROPERTY()
	FString tokenType;

	UPROPERTY()
	int64 expireIn;  //游客才有

	UPROPERTY()
	FString macAlgorithm; //tapTap才有

	void SaveToLocal()
	{
		DataStorage::SaveStruct(XDGDataStorageName::TokenInfo, *this, true);
	}

	static TSharedPtr<FTokenModel> GetLocalModel()
	{
		return  DataStorage::LoadStruct<FTokenModel>(XDGDataStorageName::TokenInfo);
	}

	static void ClearToken()
	{
		DataStorage::Remove(XDGDataStorageName::TokenInfo);
	}
	
};

USTRUCT()
struct FSyncTokenModel
{
	GENERATED_BODY()

	UPROPERTY()
	FString sessionToken;

	void SaveToLocal()
	{
		DataStorage::SaveStruct(XDGDataStorageName::SessionTokenKey, *this, true);
	}

	static TSharedPtr<FSyncTokenModel> GetLocalModel()
	{
		return  DataStorage::LoadStruct<FSyncTokenModel>(XDGDataStorageName::SessionTokenKey);
	}

	static void ClearToken()
	{
		DataStorage::Remove(XDGDataStorageName::SessionTokenKey);
	}
};



