// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "XDGEnumType.h"
#include "XDGError.h"
#include "XDGUser.h"
#include "XDGAPI.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class XDGSDK_API UXDGAPI : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, Category = "XDGSDK", meta = (DisplayName = "Get XDG SDK Event Dispatcher"))
	static const UXDGAPI*& GetXDGSDKEventDispatcher();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBool_String_CallBack, bool, Result, const FString &, Message);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUserCallBack, const FXDGUser&, User);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FErrorCallBack, const FXDGError&, Error);
	
	
	UFUNCTION(BlueprintCallable, Category = "XDGSDK", meta = (DisplayName = "Init SDK"))
	static void InitSDK(FString sdkClientId);
	UPROPERTY(BlueprintAssignable, Category = "XDGSDK CallBack")
	FBool_String_CallBack OnInitSDK;


	UFUNCTION(BlueprintCallable, Category = "XDGSDK", meta = (DisplayName = "Set Language"))
	static void SetLanguage(LangType type);

	UFUNCTION(BlueprintCallable, Category = "XDGSDK", meta = (DisplayName = "is Initialized"))
	static bool IsInitialized();

	UFUNCTION(BlueprintCallable, Category = "XDGSDK", meta = (DisplayName = "XDG Login"))
	static void LoginByType(LoginType loginType);
	UPROPERTY(BlueprintAssignable, Category = "XDGSDK CallBack")
	FUserCallBack OnLoginSuccess;
	UPROPERTY(BlueprintAssignable, Category = "XDGSDK CallBack")
	FErrorCallBack OnLoginFail;

	UFUNCTION(BlueprintCallable, Category = "XDGSDK", meta = (DisplayName = "XDG Reset Privacy"))
	static void ResetPrivacy();

	UFUNCTION(BlueprintCallable, Category = "XDGSDK", meta = (DisplayName = "XDG Logout"))
	static void Logout();

	UFUNCTION(BlueprintCallable, Category = "XDGSDK", meta = (DisplayName = "XDG Test"))
	static void Test();


private:

};
