// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "XD_PCTapDBDemo.generated.h"

/**
 * 
 */
UCLASS()
class XD_PC_API UXD_PCTapDBDemo : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "TapDBDemo", meta = (DisplayName = "Init"))
	static void Init(const TArray<FString>& Paras, const TMap<FString, FString> Properties);

	UFUNCTION(BlueprintCallable, Category = "TapDBDemo", meta = (DisplayName = "SetUser"))
	static void SetUser(const TArray<FString>& Paras, const TMap<FString, FString> Properties);

	UFUNCTION(BlueprintCallable, Category = "TapDBDemo", meta = (DisplayName = "ClearUser"))
	static void ClearUser(const TArray<FString>& Paras, const TMap<FString, FString> Properties);

	UFUNCTION(BlueprintCallable, Category = "TapDBDemo", meta = (DisplayName = "SetLevel"))
	static void SetLevel(const TArray<FString>& Paras, const TMap<FString, FString> Properties);

	UFUNCTION(BlueprintCallable, Category = "TapDBDemo", meta = (DisplayName = "SetServer"))
	static void SetServer(const TArray<FString>& Paras, const TMap<FString, FString> Properties);

	UFUNCTION(BlueprintCallable, Category = "TapDBDemo", meta = (DisplayName = "SetName"))
	static void SetName(const TArray<FString>& Paras, const TMap<FString, FString> Properties);

	UFUNCTION(BlueprintCallable, Category = "TapDBDemo", meta = (DisplayName = "ChargeSuccess"))
	static void ChargeSuccess(const TArray<FString>& Paras, const TMap<FString, FString> Properties);

	UFUNCTION(BlueprintCallable, Category = "TapDBDemo", meta = (DisplayName = "TrackEvent"))
	static void TrackEvent(const TArray<FString>& Paras, const TMap<FString, FString> Properties);

	UFUNCTION(BlueprintCallable, Category = "TapDBDemo", meta = (DisplayName = "DeviceInitialize"))
	static void DeviceInitialize(const TArray<FString>& Paras, const TMap<FString, FString> Properties);

	UFUNCTION(BlueprintCallable, Category = "TapDBDemo", meta = (DisplayName = "DeviceUpdate"))
	static void DeviceUpdate(const TArray<FString>& Paras, const TMap<FString, FString> Properties);

	UFUNCTION(BlueprintCallable, Category = "TapDBDemo", meta = (DisplayName = "DeviceAdd"))
	static void DeviceAdd(const TArray<FString>& Paras, const TMap<FString, FString> Properties);

	UFUNCTION(BlueprintCallable, Category = "TapDBDemo", meta = (DisplayName = "UserInitialize"))
	static void UserInitialize(const TArray<FString>& Paras, const TMap<FString, FString> Properties);

	UFUNCTION(BlueprintCallable, Category = "TapDBDemo", meta = (DisplayName = "UserUpdate"))
	static void UserUpdate(const TArray<FString>& Paras, const TMap<FString, FString> Properties);

	UFUNCTION(BlueprintCallable, Category = "TapDBDemo", meta = (DisplayName = "UserAdd"))
	static void UserAdd(const TArray<FString>& Paras, const TMap<FString, FString> Properties);

	UFUNCTION(BlueprintCallable, Category = "TapDBDemo", meta = (DisplayName = "RegisterStaticProperties"))
	static void RegisterStaticProperties(const TArray<FString>& Paras, const TMap<FString, FString> Properties);

	UFUNCTION(BlueprintCallable, Category = "TapDBDemo", meta = (DisplayName = "ClearStaticProperties"))
	static void ClearStaticProperties(const TArray<FString>& Paras, const TMap<FString, FString> Properties);

private:
	static TSharedPtr<FJsonObject> GetProperties(const TMap<FString, FString>& Properties)
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		for (auto Property : Properties)
		{
			if (!Property.Key.IsEmpty())
			{
				JsonObject->SetStringField(Property.Key, Property.Value);
			}
		}
		return JsonObject;
	}
};

