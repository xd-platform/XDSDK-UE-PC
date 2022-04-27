// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	void SaveToLocal();

	static TSharedPtr<FTokenModel> GetLocalModel();

	static void ClearToken();
	
};

USTRUCT()
struct FSyncTokenModel
{
	GENERATED_BODY()

	UPROPERTY()
	FString sessionToken;

	void SaveToLocal();

	static TSharedPtr<FSyncTokenModel> GetLocalModel();

	static void ClearToken();
};



