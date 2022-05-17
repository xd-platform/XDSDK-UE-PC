// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TUJsonHelper.h"
#include "XDGResponseModel.h"
#include "XDIPayCheckModel.generated.h"


USTRUCT()
struct FXDIPayCheckModel
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString tradeNo;

	UPROPERTY()
	FString outTradeNo;

	UPROPERTY()
	FString productId;

	UPROPERTY()
	FString currency;

	UPROPERTY()
	FString channelType;

	UPROPERTY()
	FString channelId;

	UPROPERTY()
	int64 supplyStatus;

	UPROPERTY()
	int64 platform;

	UPROPERTY()
	double refundAmount;

};


USTRUCT()
struct FXDIPayCheckResponseModel
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FXDIPayCheckModel> list;

};

