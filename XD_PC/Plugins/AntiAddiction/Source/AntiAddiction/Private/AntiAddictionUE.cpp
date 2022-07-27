#include "AntiAddictionUE.h"

#include "TUDebuger.h"
#include "Server/AAUImpl.h"
#include "Server/AAUStorage.h"
#include "UI/AAUHealthTipWidget.h"


void AntiAddictionUE::Init(const AAUType::Config& Config) {
	AAUImpl::Get()->Init(Config);
}

void AntiAddictionUE::StartUp(const FString& UserID, TFunction<void(AAUType::StartUpResult Result)> CallBack) {
	if (UserID.IsEmpty()) {
		TUDebuger::ErrorLog("AntiAddiction UserID is Empty");
		return;
	}
	AAUImpl::Get()->StartUp(UserID, CallBack);

}

void AntiAddictionUE::RegisterDelegate(AAUDelegate* Delegate) {
	AAUImpl::Get()->RegisterDelegate(Delegate);
}

void AntiAddictionUE::EnterGame() {
	AAUImpl::Get()->EnterGame();
}

void AntiAddictionUE::LeaveGame() {
	AAUImpl::Get()->LeaveGame();
}

void AntiAddictionUE::Logout() {
	AAUImpl::Get()->Logout();
}

int AntiAddictionUE::GetCurrentUserAgeLimite() {
	return AAUImpl::Get()->GetCurrentUserAgeLimite();
}

int AntiAddictionUE::GetCurrentUserRemainTime() {
	return AAUImpl::Get()->GetCurrentUserRemainTime();
}

void AntiAddictionUE::CheckPayLimit(int Amount,
	TFunction<void(bool Status, const FString& Title, const FString& Description)> CallBack,
	TFunction<void(const FString& Msg)> FailureHandler) {
	AAUImpl::Get()->CheckPayLimit(Amount, CallBack, FailureHandler);
}

void AntiAddictionUE::SubmitPayResult(int Amount, TFunction<void(bool Success)> CallBack,
	TFunction<void(const FString& Msg)> FailureHandler) {
	AAUImpl::Get()->SubmitPayResult(Amount, CallBack, FailureHandler);
}

void AntiAddictionUE::Test() {
	int Temp = TUDataStorage<FAAUStorage>::LoadNumber("MyTest");
	TUDebuger::DisplayShow(FString::Printf(TEXT("num: %i"), Temp));
	Temp += 1;
	TUDataStorage<FAAUStorage>::SaveNumber("MyTest", Temp);
}
