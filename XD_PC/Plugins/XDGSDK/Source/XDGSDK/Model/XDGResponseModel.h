// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JsonHelper.h"
#include "XDGResponseModel.generated.h"

// {"code":50000,"msg":"Unknown Error","detail":"Required String parameter 'clientId' is not present","data":""}
USTRUCT()
struct FXDGResponseModel
{
	GENERATED_BODY()

	UPROPERTY()
	int code;
	
	UPROPERTY()
	FString msg;

	UPROPERTY()
	FString detail;

	template <class UStructType>
	static void ParseJson(FString json, TSharedPtr<FXDGResponseModel>& model, TSharedPtr<UStructType>& data)
	{
		auto JsonObject = JsonHelper::GetJsonObject(json);
		model = JsonHelper::GetUStruct<FXDGResponseModel>(JsonObject);
		const TSharedPtr<FJsonObject>* dataJsonObject;
		if (JsonObject->TryGetObjectField("data", dataJsonObject))
		{
			data = JsonHelper::GetUStruct<UStructType>(*dataJsonObject);
		} else
		{
			data = nullptr;
		}
	}


};

