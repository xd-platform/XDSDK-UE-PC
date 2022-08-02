#pragma once
#include "TUAccessToken.h"
#include "XUError.h"
#include "XUGoogleTokenModel.h"

class XUThirdAuthHelper {
public:

	static void TapTapLogin(TFunction<void(FTUAccessToken AccessToken)> Callback, TFunction<void(FXUError Error)> ErrorBlock);

	static void GoogleLogin(TFunction<void(FXUGoogleTokenModel AccessToken)> Callback, TFunction<void(FXUError Error)> ErrorBlock);

	static void CancelAllPreAuths();

private:
	static TArray<FString> CacheStates;

	static FString GenerateState();

	static bool JudgeContainStateAndClearAllAuths(FString State);
};
