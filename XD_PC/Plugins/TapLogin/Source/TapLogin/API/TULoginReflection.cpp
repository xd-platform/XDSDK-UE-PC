// Fill out your copyright notice in the Description page of Project Settings.


#include "TULoginReflection.h"

#include "TAUProfileModel.h"


FString UTULoginReflection::GetOpenID()
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
