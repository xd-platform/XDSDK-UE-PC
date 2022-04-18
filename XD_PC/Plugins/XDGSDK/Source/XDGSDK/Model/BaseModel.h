// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseModel.generated.h"

// {"code":50000,"msg":"Unknown Error","detail":"Required String parameter 'clientId' is not present","data":""}
template <class Data>
USTRUCT()
struct FBaseModel
{
	GENERATED_BODY()

	UPROPERTY()
	int code;
	
	UPROPERTY()
	FString msg;

	UPROPERTY()
	FString detail;

	UPROPERTY()
	Data data;
};

