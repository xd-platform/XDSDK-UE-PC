// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JsonHelper.h"
#include "XDGResponseModel.h"
#include "XDGBindModel.generated.h"

USTRUCT()
struct FXDGBindModel
{
	GENERATED_BODY()

	UPROPERTY()
	int loginType;

	UPROPERTY()
	FString loginName;

	UPROPERTY()
	FString nickName;

	UPROPERTY()
	FString avatar;

	UPROPERTY()
	FString bindDate;

	UPROPERTY()
	int status;

};

USTRUCT()
struct FXDGBindResponseModel : public FXDGResponseModel
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FXDGBindModel> data;

};

