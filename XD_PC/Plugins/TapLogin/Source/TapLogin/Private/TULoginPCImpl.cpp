#include "TULoginPCImpl.h"

void TULoginPCImpl::Init(TapUELogin::Config Config) {
	TULoginImpl::Init(Config);
}

void TULoginPCImpl::ChangeLanguage(TapUELogin::LanguageType LanguageType) {
	TULoginImpl::ChangeLanguage(LanguageType);
}

TSharedPtr<FTULoginProfileModel> TULoginPCImpl::GetProfile() {
	return TULoginImpl::GetProfile();
}

void TULoginPCImpl::FetchProfile(
	TFunction<void(TSharedPtr<FTULoginProfileModel> ModelPtr, const FTUError& Error)> CallBack) {
	TULoginImpl::FetchProfile(CallBack);
}

TSharedPtr<FTUAccessToken> TULoginPCImpl::GetAccessToken() {
	return TULoginImpl::GetAccessToken();
}

void TULoginPCImpl::Login(TArray<FString> Permissions, TFunction<void(const TUAuthResult& Result)> CallBack) {
	TULoginImpl::Login(Permissions, CallBack);
}

void TULoginPCImpl::Logout() {
	TULoginImpl::Logout();
}

