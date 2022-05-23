// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "TUAccessToken.h"
#include "FProfile.h"
#include "TUError.h"
#include "TULoginFriendResult.h"
#include "Modules/ModuleManager.h"

class FTapLoginModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;


	/** RegisterLoginResultListener Callback */
	DECLARE_MULTICAST_DELEGATE_OneParam(FLoginSuccess, const FTUAccessToken);

	DECLARE_MULTICAST_DELEGATE_OneParam(FLoginError, const FTUError);

	DECLARE_MULTICAST_DELEGATE(FLoginCancel);

	UPROPERTY(BlueprintAssignable, Category = "TapLogin")
	static FLoginSuccess OnLoginSuccess;

	UPROPERTY(BlueprintAssignable, Category = "TapLogin")
	static FLoginError OnLoginError;

	UPROPERTY(BlueprintAssignable, Category = "TapLogin")
	static FLoginCancel OnLoginCancel;


	DECLARE_MULTICAST_DELEGATE_OneParam(FGetProfile, const FProfile);
	DECLARE_MULTICAST_DELEGATE_OneParam(FGetProfileError, const FTUError);

	UPROPERTY(BlueprintAssignable, Category = "TapLogin")
	static FGetProfile OnGetProfileSuccess;

	UPROPERTY(BlueprintAssignable, Category = "TapLogin")
	static FGetProfileError OnGetProfileError;

	DECLARE_MULTICAST_DELEGATE_OneParam(FGetAccessToken, const FTUAccessToken);
	DECLARE_MULTICAST_DELEGATE_OneParam(FGetAccessTokenError, const FTUError);

	UPROPERTY(BlueprintAssignable, Category= "TapLogin")
	static FGetAccessToken OnGetAccessTokenSuccess;

	UPROPERTY(BlueprintAssignable, Category = "TapLogin")
	static FGetAccessTokenError OnGetAccessTokenError;

	
	DECLARE_MULTICAST_DELEGATE_TwoParams(FQueryMutualListCallBack, const FTULoginFriendResult&, const FTUError&)
	
	UPROPERTY(BlueprintAssignable, Category = "TapLoginFriend")
	static FQueryMutualListCallBack OnQueryMutualList;

	/** GetTestQualification Callback */
	DECLARE_MULTICAST_DELEGATE_OneParam(FGetTestQualification, const bool);
	DECLARE_MULTICAST_DELEGATE_OneParam(FGetTestQualificationError, const FTUError);

	UPROPERTY(BlueprintAssignable, Category = "TapBootstrap")
	static FGetTestQualification OnGetTestQualification;

	UPROPERTY(BlueprintAssignable, Category = "TapBootstrap")
	static FGetTestQualificationError OnGetTestQualificationError;
};
