// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "XDGEnumType.h"
#include "XDGAPI.generated.h"

/**
 * 
 */
UCLASS()
class XDGSDK_API UXDGAPI : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, Category = "XDGSDK", meta = (DisplayName = "Get XDG SDK Event Dispatcher"))
	static const UXDGAPI* GetXDGSDKEventDispatcher();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBool_String_CallBack, bool, Result, FString, Message);
	
	
	UPROPERTY(BlueprintAssignable, Category = "XDGSDK CallBack")
	FBool_String_CallBack OnInitSDK;
	
	UFUNCTION(BlueprintCallable, Category = "XDGSDK", meta = (DisplayName = "Init SDK"))
	static void InitSDK(FString sdkClientId);


	UFUNCTION(BlueprintCallable, Category = "XDGSDK", meta = (DisplayName = "Set Language"))
	static void SetLanguage(LangType type);

	UFUNCTION(BlueprintCallable, Category = "XDGSDK", meta = (DisplayName = "is Initialized"))
	static bool IsInitialized();



private:

};
