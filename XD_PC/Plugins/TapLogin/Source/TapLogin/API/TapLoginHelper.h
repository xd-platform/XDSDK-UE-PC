// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TapAuthResult.h"
#include "UObject/Object.h"
#include "TapLoginHelper.generated.h"

/**
 * 
 */

namespace TapLoginPermissionScope
{
	static FString Profile = "public_profile";
	static FString Friend = "user_friends";
}


UCLASS()
class TAPLOGIN_API UTapLoginHelper : public UObject
{
	GENERATED_BODY()

public:
	
	static void Init(const FString& clientID, bool isCn, bool roundCorner);

	static void Login(TFunction<void(TapAuthResult Result)> CallBack);
	static void Login(TArray<FString> Permissions, TFunction<void(TapAuthResult Result)> CallBack);

};
