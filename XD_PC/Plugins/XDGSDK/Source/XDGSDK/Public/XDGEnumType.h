// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
 
UENUM(BlueprintType)
enum LangType
{
	ZH_CN = 0 UMETA(DisplayName = "ZH_CN"),
	ZH_TW = 1 UMETA(DisplayName = "ZH_TW"),
	EN = 2 UMETA(DisplayName = "EN"),
	TH = 3 UMETA(DisplayName = "TH"),
	ID = 4 UMETA(DisplayName = "ID"),
	KR = 5 UMETA(DisplayName = "KR"),
	JP = 6 UMETA(DisplayName = "JP"),
	DE = 7 UMETA(DisplayName = "DE"),
	FR = 8 UMETA(DisplayName = "FR"),
	PT = 9 UMETA(DisplayName = "PT"),
	ES = 10 UMETA(DisplayName = "ES"),
	TR = 11 UMETA(DisplayName = "TR"),
	RU = 12 UMETA(DisplayName = "RU"),
};

UENUM(BlueprintType)
enum class LoginType: uint8
{
	Default = uint8(-1) UMETA(DisplayName = "Default"),
	Guest  = 0 UMETA(DisplayName = "Guest"),
	TapTap = 5 UMETA(DisplayName = "TapTap"),
};

// ios的，要与后台一致！
// XDGLoginInfoTypeGuest = 0,
// XDGLoginInfoTypeWeChat = 1,
// XDGLoginInfoTypeApple = 2,
// XDGLoginInfoTypeGoogle = 3,
// XDGLoginInfoTypeFacebook = 4,
// XDGLoginInfoTypeTapTap = 5,
// XDGLoginInfoTypeLine = 6,
// XDGLoginInfoTypeTwitter = 7

