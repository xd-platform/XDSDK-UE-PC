// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "XD_PCGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class XD_PC_API AXD_PCGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
    
    UFUNCTION(BlueprintCallable, Category = "XDGameMode", meta = (DisplayName = "InitSDK"))
    static void InitSDK(const FString& ClientId, int RegionType);

    UFUNCTION(BlueprintCallable, Category = "XDGameMode", meta = (DisplayName = "LoginByType"))
    static void LoginByType(int LoginType);

    UFUNCTION(BlueprintCallable, Category = "XDGameMode", meta = (DisplayName = "IsInitialized"))
    static bool IsInitialized();

    UFUNCTION(BlueprintCallable, Category = "XDGameMode", meta = (DisplayName = "SetLanguage"))
    static void SetLanguage(int LangType);

    UFUNCTION(BlueprintCallable, Category = "XDGameMode", meta = (DisplayName = "ResetPrivacy"))
    static void ResetPrivacy();
    
    UFUNCTION(BlueprintCallable, Category = "XDGameMode", meta = (DisplayName = "Logout"))
    static void XDLogout();

    UFUNCTION(BlueprintCallable, Category = "XDGameMode", meta = (DisplayName = "Test"))
    static void Test();
    
    UFUNCTION(BlueprintCallable, Category = "XDGameMode", meta = (DisplayName = "OpenUserCenter"))
    static void OpenUserCenter();

    UFUNCTION(BlueprintCallable, Category = "XDGameMode", meta = (DisplayName = "OpenPayHintAlert"))
    static void OpenPayHintAlert();

    UFUNCTION(BlueprintCallable, Category = "XDGameMode", meta = (DisplayName = "CheckPay"))
    static void CheckPay();

    UFUNCTION(BlueprintCallable, Category = "XDGameMode", meta = (DisplayName = "OpenCustomerCenter"))
    static void OpenCustomerCenter();

    UFUNCTION(BlueprintCallable, Category = "XDGameMode", meta = (DisplayName = "OpenWebPay"))
	static void OpenWebPay(FString ServerId, FString RoleId, FString OrderId, FString ProductId, FString ProductName, float PayAmount, FString Ext);

    UFUNCTION(BlueprintCallable, Category = "XDGameMode", meta = (DisplayName = "IsPushServiceEnable"))
    static void IsPushServiceEnable();

    UFUNCTION(BlueprintCallable, Category = "XDGameMode", meta = (DisplayName = "OpenWebTopic"))
    static void OpenWebTopic(FString AppId);
	
};
