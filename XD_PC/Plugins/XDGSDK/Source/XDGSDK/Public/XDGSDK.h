// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY_STATIC(XDGSDKLog, Log, All);
#define XDG_LOG(Verbosity, Format, ...) UE_LOG(XDGSDKLog, Verbosity, Format, #__VA_ARGS__)

class FXDGSDKModule : public IModuleInterface
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
