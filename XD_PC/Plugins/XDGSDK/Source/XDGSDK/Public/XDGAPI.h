// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "XDGAPI.generated.h"

/**
 * 
 */
UCLASS()
class XDGSDK_API UXDGAPI : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, Category = "SDGSDK", meta = (DisplayName = "Get XDG SDK Event Dispatcher"))
	static const UXDGAPI* GetXDGSDKEventDispatcher();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBool_String_CallBack, bool, Result, FString, Message);
	
	
	UPROPERTY(BlueprintAssignable, Category = "SDGSDK CallBack")
	FBool_String_CallBack OnInitSDK;
	
	UFUNCTION(BlueprintCallable, Category = "SDGSDK", meta = (DisplayName = "Init SDK"))
	static void InitSDK(FString sdkClientId);


private:

};
