// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "TUAccessToken.h"
#include "TUError.h"
#include "TapUser.h"
#include "TapUserDetail.h"

class TAPBOOTSTRAP_API FTapBootstrapModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    static inline FTapBootstrapModule &Get()
    {
        return FModuleManager::LoadModuleChecked<FTapBootstrapModule>("TapBootstrap");
    }

    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("TapBootstrap");
    }

    void Init(FString tapConfig);

    /** RegisterLoginResultListener Callback */
    DECLARE_MULTICAST_DELEGATE_OneParam(FLoginSuccess, const FTapUser);

    DECLARE_MULTICAST_DELEGATE_OneParam(FLoginError, const FTUError);

    DECLARE_MULTICAST_DELEGATE(FLoginCancel);

    UPROPERTY(BlueprintAssignable, Category = "TapBootstrap")
    static FLoginSuccess OnLoginSuccess;

    UPROPERTY(BlueprintAssignable, Category = "TapBootstrap")
    static FLoginError OnLoginError;

    UPROPERTY(BlueprintAssignable, Category = "TapBootstrap")
    static FLoginCancel OnLoginCancel;

    /** RegisterUserStatusChangedListener Callback */
    DECLARE_MULTICAST_DELEGATE_OneParam(FLogout, const FTUError);

    UPROPERTY(BlueprintAssignable, Category = "TapBootstrap")
    static FLogout OnLogout;

    /** GetAccessToken Callback */
    DECLARE_MULTICAST_DELEGATE_OneParam(FGetAccessToken, const FTUAccessToken);
    
    UPROPERTY(BlueprintAssignable, Category = "TapBootstrap")
    static FGetAccessToken OnGetAccessToken;

    /** GetUser Callback */
    DECLARE_MULTICAST_DELEGATE_OneParam(FGetUserSuccess,const FTapUser&);
    DECLARE_MULTICAST_DELEGATE_OneParam(FGetUserError,const FTUError&);

    UPROPERTY(BlueprintAssignable, Category = "TapBootstrap")
    static FGetUserSuccess OnGetUserSuccess;

    UPROPERTY(BlueprintAssignable,Category = "TapBootstrap")
    static FGetUserError OnGetUserError;

    /** GetUserDetail Callback */
    DECLARE_MULTICAST_DELEGATE_OneParam(FGetUserDetailSuccess, const FTapUserDetail);
    DECLARE_MULTICAST_DELEGATE_OneParam(FGetUserDetailError, const FTUError);

    UPROPERTY(BlueprintAssignable, Category = "TapBootstrap")
    static FGetUserDetailSuccess OnGetUserDetailSuccess;

    UPROPERTY(BlueprintAssignable, Category = "TapBootstrap")
    static FGetUserDetailError OnGetUserDetailError;

    /** GetTestQualification Callback */
    DECLARE_MULTICAST_DELEGATE_OneParam(FGetTestQualification, const bool);
    DECLARE_MULTICAST_DELEGATE_OneParam(FGetTestQualificationError, const FTUError);

    UPROPERTY(BlueprintAssignable, Category = "TapBootstrap")
    static FGetTestQualification OnGetTestQualification;

    UPROPERTY(BlueprintAssignable, Category = "TapBootstrap")
    static FGetTestQualificationError OnGetTestQualificationError;
};
