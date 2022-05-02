// Fill out your copyright notice in the Description page of Project Settings.


#include "TapLoginReflection.h"

#include "TAUProfileModel.h"


FString UTapLoginReflection::GetOpenID()
{
	return FTAUProfileModel::GetLocalModel()->openid;
}
