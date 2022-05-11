// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
/*
	ZH_CN = 0, 中文
	ZH_TW = 1, 繁体中文 
	EN = 2, 英文
	TH = 3, 泰文
	ID = 4, 印尼文
	KR = 5, 韩语
	JP = 6, 日语
	DE = 7, 德语
	FR = 8, 法语
	PT = 9, 葡萄牙语
	ES = 10, 西班牙语
	TR = 11, 土耳其语
	RU = 12, 俄罗斯语
*/
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

UENUM(BlueprintType)
enum CheckPayType{
	iOS				UMETA(DisplayName = "iOS"),					//只有iOS需要补款
	Android			UMETA(DisplayName = "Android"),				//只有Android需要补款
	iOSAndAndroid	UMETA(DisplayName = "iOS And Android"),		//iOS Android 都需要补款
	None			UMETA(DisplayName = "None"),				//没有要补款
};

