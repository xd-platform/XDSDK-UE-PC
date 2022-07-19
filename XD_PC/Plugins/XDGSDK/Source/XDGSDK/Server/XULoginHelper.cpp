#include "XULoginHelper.h"
#include "TapUELogin.h"


void XULoginHelper::TapTapLogin(TFunction<void(FTUAccessToken AccessToken)> Callback,
                                TFunction<void(FXUError Error)> ErrorBlock) {
	TapUELogin::Login(
		[=](TUAuthResult Result) {
			if (Result.GetType() == TUAuthResult::Success) {
				Callback(*Result.GetToken().Get());
			}
			else if (Result.GetType() == TUAuthResult::Cancel) {
				FXUError Error;
				Error.msg = "Login Cancel";
				Error.code = FTUError::ERROR_CODE_LOGIN_CANCEL;
				ErrorBlock(Error);
			}
			else if (Result.GetType() == TUAuthResult::Fail) {
				FXUError Error;
				Error.msg = Result.GetError()->error_description;
				Error.code = Result.GetError()->code;
				ErrorBlock(Error);
			}
			else {
				ErrorBlock(FXUError("Login Fail"));
			}
		});
}
