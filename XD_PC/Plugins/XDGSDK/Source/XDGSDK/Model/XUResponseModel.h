// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TUJsonHelper.h"
#include "XUResponseModel.generated.h"

// {"code":50000,"msg":"Unknown Error","detail":"Required String parameter 'clientId' is not present","data":""}
USTRUCT()
struct FXUResponseModel
{
	GENERATED_BODY()

	UPROPERTY()
	int code;
	
	UPROPERTY()
	FString msg;

	UPROPERTY()
	FString detail;

	template <class UStructType>
	static void ParseJson(const FString& json, TSharedPtr<FXUResponseModel>& model, TSharedPtr<UStructType>& data)
	{
		auto JsonObject = TUJsonHelper::GetJsonObject(json);
		if (!JsonObject.IsValid()) {
			model = nullptr;
			data = nullptr;
			return;
		}
		model = TUJsonHelper::GetUStruct<FXUResponseModel>(JsonObject);
		const TSharedPtr<FJsonObject>* dataJsonObject;
		if (JsonObject->TryGetObjectField("data", dataJsonObject))
		{
			data = TUJsonHelper::GetUStruct<UStructType>(*dataJsonObject);
		} else
		{
			data = nullptr;
		}
	}


};

