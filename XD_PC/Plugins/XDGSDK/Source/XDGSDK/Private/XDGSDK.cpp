// Copyright Epic Games, Inc. All Rights Reserved.

#include "XDGSDK.h"

DEFINE_LOG_CATEGORY(XDGSDKLog);

#define LOCTEXT_NAMESPACE "FXDGSDKModule"

int FXDGSDKModule::Version = 60101001;
FString FXDGSDKModule::VersionName = "6.1.1";

void FXDGSDKModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FXDGSDKModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FXDGSDKModule, XDGSDK)