// Fill out your copyright notice in the Description page of Project Settings.


#include "TapBootstrapAPI.h"
#include "TapUELogin.h"

void UTapBootstrap::Init(const FTUCommonConfig& tapConfig)
{
	TapUELogin::Config Config;
	Config.ClientID = tapConfig.clientID;
	if (tapConfig.regionType == RegionType::CN) {
		Config.RegionType = TapUELogin::CN;
	}
	else {
		Config.RegionType = TapUELogin::IO;
	}
	TapUELogin::Init(Config);
}
