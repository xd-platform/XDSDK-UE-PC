// Fill out your copyright notice in the Description page of Project Settings.


#include "TapLoginHelper.h"

#include "TapLoginImpl.h"


void UTapLoginHelper::Init(const FString& clientID, bool isCn, bool roundCorner)
{
	TapLoginImpl::Get().Init(clientID, isCn, roundCorner);
}
