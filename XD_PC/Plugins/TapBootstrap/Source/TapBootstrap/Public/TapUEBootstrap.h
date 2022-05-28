#pragma once
#include "TapUser.h"
#include "TUError.h"
#include "TUType.h"

class TapUEBootstrap {
public:

	// 只能初始化一次
	static void Init(const TUType::Config& Config);

	static void Login(TArray<FString> Permissions, TFunction<void(const FTapUser& User)> SuccessBlock, TFunction<void(const FTUError& Error)> FailBlock);

	static void AnonymouslyLogin(TFunction<void(const FTapUser& User)> SuccessBlock, TFunction<void(const FTUError& Error)> FailBlock);

	static void Logout();

	static TSharedPtr<FTapUser> GetUser();

	static void SetPreferLanguage(TUType::LanguageType LangType);

	// static void GetDetailUser();
	//
	// static void GetAccessToken();
	//
	// static void OpenUserCenter();
	//
	// static void GetTestQualification();
	
};
