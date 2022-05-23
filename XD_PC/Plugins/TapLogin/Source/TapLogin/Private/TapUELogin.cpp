#include "TapUELogin.h"

#include "TUDebuger.h"
#include "TULoginImpl.h"

FString TapUELogin::PermissionScope::Profile = "public_profile";
FString TapUELogin::PermissionScope::Friend = "user_friends";

static bool IsInitialized = false;

void TapUELogin::Init(Config Config) {
	if (IsInitialized) {
		TUDebuger::DisplayLog("Has Initialized");
		return;
	}
	IsInitialized = true;
	TULoginImpl::Get()->Init(Config);
}

TSharedPtr<FTULoginProfileModel> TapUELogin::GetProfile() {
	checkf(IsInitialized, TEXT("Please Init First"));
	return TULoginImpl::Get()->GetProfile();
}

void TapUELogin::FetchProfile(
	TFunction<void(TSharedPtr<FTULoginProfileModel> ModelPtr, const FTUError& Error)> CallBack) {
	checkf(IsInitialized, TEXT("Please Init First"));
	TULoginImpl::Get()->FetchProfile(CallBack);
}

TSharedPtr<FTUAccessToken> TapUELogin::GetAccessToken() {
	checkf(IsInitialized, TEXT("Please Init First"));
	return TULoginImpl::Get()->GetAccessToken();
}

void TapUELogin::Login(TFunction<void(const TUAuthResult& Result)> CallBack) {
	checkf(IsInitialized, TEXT("Please Init First"));
	Login({PermissionScope::Profile}, CallBack);
}

void TapUELogin::Login(TArray<FString> Permissions, TFunction<void(const TUAuthResult& Result)> CallBack) {
	checkf(IsInitialized, TEXT("Please Init First"));
	TULoginImpl::Get()->Login(Permissions, CallBack);
}

void TapUELogin::Logout() {
	checkf(IsInitialized, TEXT("Please Init First"));
	TULoginImpl::Get()->Logout();
}

#if PLATFORM_MAC || PLATFORM_WINDOWS
	
void TapUELogin::ChangeLanguage(LanguageType LanguageType) {
	checkf(IsInitialized, TEXT("Please Init First"));
	TULoginImpl::Get()->ChangeLanguage(LanguageType);
}

#elif PLATFORM_IOS || PLATFORM_ANDROID
	
void TapUELogin::GetTestQualification(TFunction<void(bool IsQualified, const FTUError& Error)> CallBack) {
	checkf(IsInitialized, TEXT("Please Init First"));
	TULoginImpl::Get()->GetTestQualification(CallBack);
}

void TapUELogin::QueryMutualList(FString Cursor, int Size,
	TFunction<void(TSharedPtr<FTULoginFriendResult> ModelPtr, const FTUError& Error)> CallBack) {
	checkf(IsInitialized, TEXT("Please Init First"));
	TULoginImpl::Get()->QueryMutualList(Cursor, Size, CallBack);
}
#endif
