#pragma once
#include "TUAccessToken.h"
#include "XUError.h"
#include "XUGoogleTokenModel.h"

class XULoginHelper {
public:

	static void TapTapLogin(TFunction<void(FTUAccessToken AccessToken)> Callback, TFunction<void(FXUError Error)> ErrorBlock);

	static void GoogleLogin(TFunction<void(FXUGoogleTokenModel AccessToken)> Callback, TFunction<void(FXUError Error)> ErrorBlock);

private:
	static FString HelperState;
};
