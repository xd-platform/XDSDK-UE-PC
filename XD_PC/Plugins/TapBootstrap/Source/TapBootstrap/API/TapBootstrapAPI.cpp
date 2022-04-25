// Fill out your copyright notice in the Description page of Project Settings.


#include "TapBootstrapAPI.h"

#include "TapLoginHelper.h"

void UTapBootstrap::Init(const FTapConfig& tapConfig)
{
	UTapLoginHelper::Init(tapConfig.clientID, tapConfig.regionType == RegionType::CN, false);
}
