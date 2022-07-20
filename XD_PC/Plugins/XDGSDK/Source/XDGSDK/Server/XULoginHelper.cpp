#include "XULoginHelper.h"

#include "HttpServerResponse.h"
#include "TapUELogin.h"
#include "TUCrypto.h"
#include "TUHelper.h"
#include "TUHttpManager.h"
#include "TUHttpRequest.h"
#include "TUHttpServer.h"
#include "TUJsonHelper.h"
#include "TUOpenSSL.h"
#include "XUConfigManager.h"
#include "XUGoogleTokenModel.h"


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

void XULoginHelper::GoogleLogin(TFunction<void(FXUGoogleTokenModel AccessToken)> Callback,
	TFunction<void(FXUError Error)> ErrorBlock) {
	FString State = FGuid::NewGuid().ToString();
	FString CodeVerifier = TUHelper::GetRandomStr(16);
	FString ClientID = XUConfigManager::CurrentConfig()->GoogleInfo.ClientID;
	FString ClientSecret = XUConfigManager::CurrentConfig()->GoogleInfo.ClientSecret;
	static FString RedirectUri = "";
	RedirectUri = TUHttpServer::RegisterNewRoute("google_auth", [=](const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete) {
		
		FString WebState = Request.QueryParams.FindRef("state");
		FString WebCode = Request.QueryParams.FindRef("code");
		if (WebState != State) {
			return false;
		}
		// TUDebuger::ErrorLog("HAHA, wolaile");
		TUniquePtr<FHttpServerResponse> ResponsePtr = MakeUnique<FHttpServerResponse>();
		ResponsePtr->Code = EHttpServerResponseCodes::Ok;
		ResponsePtr->Headers.Add("Content-Type", {"text/plain"});
		ResponsePtr->Headers.Add("Access-Control-Allow-Origin", {"*"});
		if (WebCode.IsEmpty()) {
			ResponsePtr->Body.Append(TUCrypto::UTF8Encode(FString("Auth Fail")));
			if (ErrorBlock) {
				ErrorBlock(FXUError("Google Auth Fail"));
			}
			OnComplete(MoveTemp(ResponsePtr));
			return true;
		}
		TSharedPtr<TUHttpRequest> TokenRequest = MakeShareable(new TUHttpRequest());
		TokenRequest->URL = "https://oauth2.googleapis.com/token";
		TokenRequest->Type = TUHttpRequest::Post;
		TokenRequest->Parameters->SetStringField("client_id", ClientID);
		TokenRequest->Parameters->SetStringField("client_secret", ClientSecret);
		TokenRequest->Parameters->SetStringField("code", WebCode);
		TokenRequest->Parameters->SetStringField("code_verifier", CodeVerifier);
		TokenRequest->Parameters->SetStringField("grant_type", "authorization_code");
		TokenRequest->Parameters->SetStringField("redirect_uri", RedirectUri);
		TokenRequest->onCompleted.BindLambda([=](TSharedPtr<TUHttpResponse> response) {
			if (response->state == TUHttpResponse::success) {
				auto ModelPtr = TUJsonHelper::GetUStruct<FXUGoogleTokenModel>(response->contentString);
				if (ModelPtr.IsValid()) {
					if (Callback) {
						Callback(*ModelPtr.Get());
					}
				}
				else {
					if (ErrorBlock) {
						ErrorBlock(FXUError("Parse Google Token Error"));
					}
				}
			}
			else {
				if (ErrorBlock) {
					ErrorBlock(FXUError(response->contentString));
				}
			}
		});
		TUHttpManager::Get().request(TokenRequest);
		
		ResponsePtr->Body.Append(TUCrypto::UTF8Encode(FString("OK")));
		OnComplete(MoveTemp(ResponsePtr));
		return true;
	});
	if (RedirectUri.IsEmpty()) {
		if (ErrorBlock) {
			ErrorBlock(FXUError("Login Server Create Error"));
		}
	} else {
		
		TSharedPtr<FJsonObject> Paras = MakeShareable(new FJsonObject);
		Paras->SetStringField("client_id", ClientID);
		Paras->SetStringField("redirect_uri", RedirectUri);
		Paras->SetStringField("response_type", "code");
		Paras->SetStringField("scope", "openid https://www.googleapis.com/auth/userinfo.profile https://www.googleapis.com/auth/userinfo.email");
		Paras->SetStringField("code_challenge", TUCrypto::UrlBase64Encode(TUOpenSSL::Sha256(TUCrypto::UTF8Encode(CodeVerifier))));
		Paras->SetStringField("code_challenge_method", "S256");
		Paras->SetStringField("state", State);
		
		FString ParaStr = TUHelper::CombinParameters(Paras);
		FString URL = "https://accounts.google.com/o/oauth2/v2/auth?" + ParaStr;
		FPlatformProcess::LaunchURL(*URL, nullptr, nullptr);
	}
	
}
