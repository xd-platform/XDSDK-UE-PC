// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XUGoogleTokenModel.generated.h"


USTRUCT()
struct FXUGoogleTokenModel
{
	GENERATED_BODY()
	
	// UPROPERTY()
	// FString access_token;
	//
	// UPROPERTY()
	// int expires_in;
	//
	// UPROPERTY()
	// FString refresh_token;
	//
	// UPROPERTY()
	// int64 scope;
	//
	// UPROPERTY()
	// FString token_type;
	//
	// UPROPERTY()
	// FString id_token;

	UPROPERTY()
	FString code;
};





