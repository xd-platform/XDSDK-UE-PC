// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XDGError.generated.h"

USTRUCT()
struct FXDGError
{
	GENERATED_BODY()

	UPROPERTY()
	int code = 0;
	
	UPROPERTY()
	FString msg;

};

