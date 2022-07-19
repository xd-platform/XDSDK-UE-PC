#pragma once
#include "TUAccessToken.h"
#include "XUError.h"

class XULoginHelper {
public:

	static void TapTapLogin(TFunction<void(FTUAccessToken AccessToken)> Callback, TFunction<void(FXUError Error)> ErrorBlock);

	
};
