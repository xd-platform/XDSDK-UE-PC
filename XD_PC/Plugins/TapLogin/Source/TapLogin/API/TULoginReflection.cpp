// Fill out your copyright notice in the Description page of Project Settings.


#include "TULoginReflection.h"

#include "TULoginProfileModel.h"


FString UTULoginReflection::GetOpenID()
{
	auto Profile = FTULoginProfileModel::GetLocalModel();
	if (Profile.IsValid())
	{
		return Profile->openid;
	} else
	{
		return "";
	}
}
