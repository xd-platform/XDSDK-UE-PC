#include "TULoginPCImpl.h"

#include "Server/TULoginNet.h"
#include "UI/TAULoginWidget.h"

void TULoginPCImpl::Init(TapUELogin::Config _Config) {
	this->Config = _Config;
}

void TULoginPCImpl::ChangeLanguage(TapUELogin::LanguageType LanguageType) {
	TULoginImpl::ChangeLanguage(LanguageType);
}

TSharedPtr<FTULoginProfileModel> TULoginPCImpl::GetProfile() {
	return FTULoginProfileModel::GetLocalModel();
}

void TULoginPCImpl::FetchProfile(
	TFunction<void(TSharedPtr<FTULoginProfileModel> ModelPtr, const FTUError& Error)> CallBack) {

	auto AccessTokenPtr = GetAccessToken();
	if (!AccessTokenPtr.IsValid() && CallBack != nullptr) {
		CallBack(nullptr, FTUError(-1, "no login"));
		return;
	}
	TULoginNet::RequestProfile(*AccessTokenPtr.Get(), [=](TSharedPtr<FTULoginProfileModel> Model, FTULoginError Error) {
		if (CallBack) {
			CallBack(Model, FTUError(Error.code, Error.msg));
		}
	});
}

TSharedPtr<FTUAccessToken> TULoginPCImpl::GetAccessToken() {
	return FTUAccessToken::GetLocalModel();
}

void TULoginPCImpl::Login(TArray<FString> Permissions, TFunction<void(const TUAuthResult& Result)> CallBack) {
	UTAULoginWidget::ShowLoginUI(Permissions, CallBack);
}

void TULoginPCImpl::Logout() {
	FTULoginProfileModel::ClearLocalModel();
	FTUAccessToken::ClearLocalModel();
}