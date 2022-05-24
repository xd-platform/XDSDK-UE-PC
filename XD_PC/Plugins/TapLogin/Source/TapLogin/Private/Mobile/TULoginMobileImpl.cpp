#include "TULoginMobileImpl.h"

void TULoginMobileImpl::Init(TapUELogin::Config _Config) {
	TULoginImpl::Init(Config);
}

TSharedPtr<FTULoginProfileModel> TULoginMobileImpl::GetProfile() {
	return TULoginImpl::GetProfile();
}

void TULoginMobileImpl::FetchProfile(
	TFunction<void(TSharedPtr<FTULoginProfileModel> ModelPtr, const FTUError& Error)> CallBack) {
	TULoginImpl::FetchProfile(CallBack);
}

TSharedPtr<FTUAccessToken> TULoginMobileImpl::GetAccessToken() {
	return TULoginImpl::GetAccessToken();
}

void TULoginMobileImpl::Login(TArray<FString> Permissions, TFunction<void(const TUAuthResult& Result)> CallBack) {
	TULoginImpl::Login(Permissions, CallBack);
}

void TULoginMobileImpl::Logout() {
	TULoginImpl::Logout();
}

void TULoginMobileImpl::GetTestQualification(TFunction<void(bool IsQualified, const FTUError& Error)> CallBack) {
	TULoginImpl::GetTestQualification(CallBack);
}

void TULoginMobileImpl::QueryMutualList(FString Cursor, int Size,
	TFunction<void(TSharedPtr<FTULoginFriendResult> ModelPtr, const FTUError& Error)> CallBack) {
	TULoginImpl::QueryMutualList(Cursor, Size, CallBack);
}
