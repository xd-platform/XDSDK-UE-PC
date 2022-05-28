// Fill out your copyright notice in the Description page of Project Settings.


#include "TapBootstrapAPI.h"
#include "TapUELogin.h"

void UTapBootstrap::Init(const FTUCommonConfig& tapConfig)
{
	TULoginType::Config Config;
	Config.ClientID = tapConfig.clientID;
	if (tapConfig.regionType == RegionType::CN) {
		Config.RegionType = TULoginType::CN;
	}
	else {
		Config.RegionType = TULoginType::IO;
	}
	TapUELogin::Init(Config);
}
