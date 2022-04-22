// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TapLoginHelper.generated.h"

/**
 * 
 */
UCLASS()
class TAPLOGIN_API UTapLoginHelper : public UObject
{
	GENERATED_BODY()

public:
	static void Init(const FString& clientID, bool isCn, bool roundCorner);

};
