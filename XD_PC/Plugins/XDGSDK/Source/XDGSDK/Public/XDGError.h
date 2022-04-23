// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XDGError.generated.h"

USTRUCT(BlueprintType)
struct FXDGError
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int code = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString msg;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString detail;

	FXDGError() = default;
	FXDGError(const FString& message) :code(-1), msg(message) {};
};

