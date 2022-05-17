// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TapCommon/Model/TUCommonConfig.h"
#include "UObject/Object.h"
#include "TapBootstrapAPI.generated.h"

/**
 * 
 */
UCLASS()
class TAPBOOTSTRAP_API UTapBootstrap : public UObject
{
	GENERATED_BODY()
public:
	static void Init(const FTUCommonConfig& tapConfig);
};
