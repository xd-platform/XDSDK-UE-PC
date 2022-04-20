// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
XDGSDK_API DECLARE_LOG_CATEGORY_EXTERN(XDGSDKLog, Log, All);

class XDGSDK_API FXDGSDKModule : public IModuleInterface
{
public:
	// e.g 60101001
	static int Version;
	// e.g "6.1.1"
	static FString VersionName;


	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
