// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XUTokenModel.h"
#include "XUType.h"
#include "XUUser.generated.h"

USTRUCT(BlueprintType)
struct FXUUser
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString userId;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString username;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString nickName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int loginType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FString> loginList; // boundAccounts

	void SaveToLocal();

	static TSharedPtr<FXUUser>& GetLocalModel();
	
	TSharedPtr<FXUTokenModel> Token();

	XUType::LoginType GetLoginType();

	static void ClearUserData();
	
	static bool IsPushServiceEnable();

	static void SetPushServiceEnable(bool enable);

private:
	static TSharedPtr<FXUUser> CurrentModel;

};

