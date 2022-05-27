#include "TapUELogin.h"
#include "TUDebuger.h"
#include "TULoginImpl.h"

FString TapUELogin::PermissionScope::Profile = "public_profile";
FString TapUELogin::PermissionScope::Friend = "user_friends";

static bool IsInitialized = false;

void TapUELogin::Init(Config Config) {
	if (IsInitialized) {
		TUDebuger::WarningShow("Has Initialized");
		return;
	}
	IsInitialized = true;
	TULoginImpl::Get()->Init(Config);
}

TSharedPtr<FTULoginProfileModel> TapUELogin::GetProfile() {
	if (!IsInitialized) {
		TUDebuger::WarningShow("Please Init First");
		return nullptr;
	}
	return TULoginImpl::Get()->GetProfile();
}

void TapUELogin::FetchProfile(
	TFunction<void(TSharedPtr<FTULoginProfileModel> ModelPtr, const FTUError& Error)> CallBack) {
	if (!IsInitialized) {
		TUDebuger::WarningShow("Please Init First");
		return;
	}
	TULoginImpl::Get()->FetchProfile(CallBack);
}

TSharedPtr<FTUAccessToken> TapUELogin::GetAccessToken() {
	if (!IsInitialized) {
		TUDebuger::WarningShow("Please Init First");
		return nullptr;
	}
	return TULoginImpl::Get()->GetAccessToken();
}

void TapUELogin::Login(TFunction<void(const TUAuthResult& Result)> CallBack) {
	if (!IsInitialized) {
		TUDebuger::WarningShow("Please Init First");
		return;
	}
	Login({PermissionScope::Profile}, CallBack);
}

void TapUELogin::Login(TArray<FString> Permissions, TFunction<void(const TUAuthResult& Result)> CallBack) {
	if (!IsInitialized) {
		TUDebuger::WarningShow("Please Init First");
		return;
	}
	TULoginImpl::Get()->Login(Permissions, CallBack);
}

void TapUELogin::Logout() {
	if (!IsInitialized) {
		TUDebuger::WarningShow("Please Init First");
		return ;
	}
	TULoginImpl::Get()->Logout();
}

	
void TapUELogin::ChangeLanguage(LanguageType LanguageType) {
	if (!IsInitialized) {
		TUDebuger::WarningShow("Please Init First");
		return;
	}
	TULoginImpl::Get()->ChangeLanguage(LanguageType);
}
	
void TapUELogin::GetTestQualification(TFunction<void(bool IsQualified, const FTUError& Error)> CallBack) {
	if (!IsInitialized) {
		TUDebuger::WarningShow("Please Init First");
		return;
	}
	TULoginImpl::Get()->GetTestQualification(CallBack);
}

void TapUELogin::QueryMutualList(FString Cursor, int Size,
	TFunction<void(TSharedPtr<FTULoginFriendResult> ModelPtr, const FTUError& Error)> CallBack) {
	if (!IsInitialized) {
		TUDebuger::WarningShow("Please Init First");
		return;
	}
	TULoginImpl::Get()->QueryMutualList(Cursor, Size, CallBack);
}