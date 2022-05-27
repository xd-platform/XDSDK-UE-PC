// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

static XDGSDK_API int XDUESDK_VERSION_NUMBER = 60101001;
static XDGSDK_API FString XDUESDK_VERSION = "6.1.1";


class XDGSDK_API FXDGSDKModule : public IModuleInterface
{
public:
	
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
