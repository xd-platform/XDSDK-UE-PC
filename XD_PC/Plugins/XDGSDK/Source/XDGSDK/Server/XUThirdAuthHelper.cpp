#include "XUThirdAuthHelper.h"

#include "HttpServerResponse.h"
#include "TapUELogin.h"
#include "TUCrypto.h"
#include "TUDeviceInfo.h"
#include "TUHelper.h"
#include "TUHttpServer.h"
#include "XUConfigManager.h"
#include "XUGoogleTokenModel.h"
#include "XULanguageManager.h"


// 用于判断是否属于同一次授权，授权前生成一个State，回调时候判断一下State和当前state是否相等，如果相等，表示的是处理当前回调，然后将HelperState置为空，否则不处理当前回调。
// 须知：三方授权的web登录（除taptap），state缓存在系统浏览器中是同一个字段，所以每次web回调，给的都是最后一次传给web的state
TArray<FString> XUThirdAuthHelper::CacheStates;

void XUThirdAuthHelper::TapTapLogin(TFunction<void(FTUAccessToken AccessToken)> Callback,
                                TFunction<void(FXUError Error)> ErrorBlock) {
	FString CurrentState = GenerateState();
	TapUELogin::Login(
		[=](TUAuthResult Result) {
			if (!JudgeContainStateAndClearAllAuths(CurrentState)) {
				return;
			}
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

void XUThirdAuthHelper::GoogleLogin(TFunction<void(FXUGoogleTokenModel AccessToken)> Callback,
	TFunction<void(FXUError Error)> ErrorBlock) {
	static FString RedirectUri = "";
	RedirectUri = TUHttpServer::RegisterNewRoute("google_auth", [=](const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete) {
		FString WebState = Request.QueryParams.FindRef("state");
		if (!JudgeContainStateAndClearAllAuths(WebState)) {
			return false;
		}
		
		TUniquePtr<FHttpServerResponse> ResponsePtr = MakeUnique<FHttpServerResponse>();
		ResponsePtr->Code = EHttpServerResponseCodes::Ok;
		ResponsePtr->Headers.Add("Content-Type", {"text/plain"});
		ResponsePtr->Headers.Add("Access-Control-Allow-Origin", {"*"});

		FString WebCode = Request.QueryParams.FindRef("code");
		if (WebCode.IsEmpty()) {
			ResponsePtr->Code = EHttpServerResponseCodes::Forbidden;
			ResponsePtr->Body.Append(TUCrypto::UTF8Encode(FString("Auth Fail")));
			if (ErrorBlock) {
				ErrorBlock(FXUError("Google Auth Fail"));
			}
			OnComplete(MoveTemp(ResponsePtr));
			return true;
		}
		FXUGoogleTokenModel GoogleToken;
		GoogleToken.code = WebCode;
		if (Callback) {
			Callback(GoogleToken);
		}
		ResponsePtr->Body.Append(TUCrypto::UTF8Encode(FString("OK")));
		OnComplete(MoveTemp(ResponsePtr));
		return true;
	});
	if (RedirectUri.IsEmpty()) {
		if (ErrorBlock) {
			ErrorBlock(FXUError("Login Server Create Error"));
		}
	} else {
		FString State = GenerateState();
		FString ClientID = XUConfigManager::CurrentConfig()->GoogleInfo.ClientID;
		
		TSharedPtr<FJsonObject> Paras = MakeShareable(new FJsonObject);
		Paras->SetStringField("client_id", ClientID);
		Paras->SetStringField("redirect_uri", RedirectUri);
		Paras->SetStringField("game_name", XUConfigManager::CurrentConfig()->GameName);
		Paras->SetStringField("lang", XULanguageManager::GetLanguageKey());
		Paras->SetStringField("state", State);
		Paras->SetStringField("from", "UE_" + TUDeviceInfo::GetPlatform());
		Paras->SetStringField("auth_type", "google");
		
		FString ParaStr = TUHelper::CombinParameters(Paras);
		FString URL = "https://xd-website.oss-cn-beijing.aliyuncs.com/xd-order-sgp/v1.0-dev/test/index.html?" + ParaStr;
		FPlatformProcess::LaunchURL(*URL, nullptr, nullptr);
	}
	
}

void XUThirdAuthHelper::CancelAllPreAuths() {
	CacheStates.Empty();
}

FString XUThirdAuthHelper::GenerateState() {
	FString State = FGuid::NewGuid().ToString();
	CacheStates.Add(State);
	return State;
}

bool XUThirdAuthHelper::JudgeContainStateAndClearAllAuths(FString State) {
	if (CacheStates.Contains(State)) {
		CancelAllPreAuths();
		return true;
	}
	return false;
}
