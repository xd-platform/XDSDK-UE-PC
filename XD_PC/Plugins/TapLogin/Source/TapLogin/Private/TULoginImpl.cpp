#include "TULoginImpl.h"
#include "TUDebuger.h"
#include "TULanguage.h"
#include "Desktop/TAULoginLanguage.h"
#if PLATFORM_MAC || PLATFORM_WINDOWS
#include "Desktop/TULoginPCImpl.h"
#elif PLATFORM_IOS || PLATFORM_ANDROID
#include "Mobile/TULoginMobileImpl.h"
#endif

TSharedPtr<TULoginImpl> TULoginImpl::Instance = nullptr;

TSharedPtr<TULoginImpl>& TULoginImpl::Get() {
	if (!Instance.IsValid()) {
#if PLATFORM_MAC || PLATFORM_WINDOWS
		Instance = MakeShareable(new TULoginPCImpl);
#elif PLATFORM_IOS || PLATFORM_ANDROID
		Instance = MakeShareable(new TULoginMobileImpl);
#else
		Instance = MakeShareable(new TULoginImpl);
#endif
	}
	return Instance;
}

void TULoginImpl::Init(TULoginType::Config _Config) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

TSharedPtr<FTULoginProfileModel> TULoginImpl::GetProfile() {
	TUDebuger::ErrorLog("Unsupported Platforms");
	return nullptr;
}

void TULoginImpl::FetchProfile(TFunction<void(TSharedPtr<FTULoginProfileModel> ModelPtr, const FTUError& Error)> CallBack) {
	TUDebuger::ErrorLog("Unsupported Platforms");
	if (CallBack) {
		CallBack(nullptr, FTUError(FTUError::ERROR_CODE_LOGIN_Platforms_Unsupported, "Unsupported Platforms"));
	}
}

TSharedPtr<FTUAccessToken> TULoginImpl::GetAccessToken() {
	TUDebuger::ErrorLog("Unsupported Platforms");
	return nullptr;
}

void TULoginImpl::Login(TArray<FString> Permissions, TFunction<void(const TUAuthResult& Result)> CallBack) {
	TUDebuger::ErrorLog("Unsupported Platforms");
	if (CallBack) {
		TUAuthResult Result = TUAuthResult::FailInit(FTUError(FTUError::ERROR_CODE_LOGIN_Platforms_Unsupported, "Unsupported Platforms"));
		CallBack(Result);
	}
}

void TULoginImpl::Logout() {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TULoginImpl::ChangeLanguage(TUType::LanguageType LanguageType) {
	switch (LanguageType) {
	TULanguage::GetCurrentType();
	case TUType::AUTO:
		if (Config.RegionType == TUType::CN) {
			TAULoginLanguage::SetLangType(TAULoginLanguage::CN);
		}
		else {
			TAULoginLanguage::SetLangType(TAULoginLanguage::EN);
		}
		break;
	case TUType::ZH:
		TAULoginLanguage::SetLangType(TAULoginLanguage::CN);
		break;
	case TUType::EN:
		TAULoginLanguage::SetLangType(TAULoginLanguage::EN);
		break;
	case TUType::ZHTW:
		TAULoginLanguage::SetLangType(TAULoginLanguage::ZHTW);
		break;
	case TUType::JA:
		TAULoginLanguage::SetLangType(TAULoginLanguage::JA);
		break;
	case TUType::KO:
		TAULoginLanguage::SetLangType(TAULoginLanguage::KO);
		break;
	case TUType::TH:
		TAULoginLanguage::SetLangType(TAULoginLanguage::TH);
		break;
	case TUType::ID:
		TAULoginLanguage::SetLangType(TAULoginLanguage::ID);
		break;
	default: ;
	}
	
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TULoginImpl::GetTestQualification(TFunction<void(bool IsQualified, const FTUError& Error)> CallBack) {
	TUDebuger::ErrorLog("Unsupported Platforms");
	if (CallBack) {
		CallBack(false, FTUError(FTUError::ERROR_CODE_LOGIN_Platforms_Unsupported, "Unsupported Platforms"));
	}
}

void TULoginImpl::QueryMutualList(FString Cursor, int Size,
	TFunction<void(TSharedPtr<FTULoginFriendResult> ModelPtr, const FTUError& Error)> CallBack) {
	TUDebuger::ErrorLog("Unsupported Platforms");
	if (CallBack) {
		CallBack(nullptr, FTUError(FTUError::ERROR_CODE_LOGIN_Platforms_Unsupported, "Unsupported Platforms"));
	}
}
