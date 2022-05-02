// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TapLoginReflection.generated.h"

/**
 * 
 */
UCLASS()
class TAPLOGIN_API UTapLoginReflection : public UObject
{
	GENERATED_BODY()

	UFUNCTION()
	static FString GetOpenID();
};
