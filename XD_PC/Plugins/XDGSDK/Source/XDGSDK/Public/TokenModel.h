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

	static TSharedPtr<FTokenModel> GetCurrentToken();

	static void SetCurrentToken(TSharedPtr<FTokenModel> model);

private:
	static TSharedPtr<FTokenModel> currentToken;
};

USTRUCT()
struct FSyncTokenModel
{
	GENERATED_BODY()

	UPROPERTY()
	FString sessionToken;
};



