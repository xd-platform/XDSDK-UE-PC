// Copyright Epic Games, Inc. All Rights Reserved.

#include "TapWebBrowserModule.h"
#include "Modules/ModuleManager.h"
#include "WebBrowserModule.h"
#include "IWebBrowserSingleton.h"
#include "Materials/Material.h"

//////////////////////////////////////////////////////////////////////////

class FTapWebBrowserModule : public ITapWebBrowserModule
{
public:
	virtual void StartupModule() override
	{
		
	}

	virtual void ShutdownModule() override
	{
	}
	
private:
	
};

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_MODULE(FTapWebBrowserModule, TapWebBrowser);
