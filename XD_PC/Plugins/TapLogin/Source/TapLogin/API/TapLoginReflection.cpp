// Fill out your copyright notice in the Description page of Project Settings.


#include "TapLoginReflection.h"

#include "TAUProfileModel.h"


FString UTapLoginReflection::GetOpenID()
{
	auto Profile = FTAUProfileModel::GetLocalModel();
	if (Profile.IsValid())
	{
		return Profile->openid;
	} else
	{
		return "";
	}
}
