#include "TauWebAuthHelper.h"

#include "HttpServerModule.h"
#include "HttpServerResponse.h"
#include "TULoginImpl.h"
#include "TUCrypto.h"
#include "TUHelper.h"
#include "TUDebuger.h"
#include "Server/TULoginRegionConfig.h"
#include "TUOpenSSL.h"

static FString WebAuthPath = "authorize";

TauWebAuthHelper::TauWebAuthHelper() {
	State = FGuid::NewGuid().ToString();
	CodeVerifier = TUHelper::GetRandomStr(16);
}

TauWebAuthHelper::~TauWebAuthHelper() {
	StopProcess();
}

bool TauWebAuthHelper::ProcessWebAuth(const TArray<FString>& NeedPermissions,
	TFunction<void(FString WebCode)> CallBack) {
	this->Permissions = NeedPermissions;
	this->CallBackBlock = CallBack;
	if (GetHttpRouter() && SetAuthHandle()) {
		FPlatformProcess::LaunchURL(*GenerateWebAuthUrl(), nullptr, nullptr);
		return true;
	} else {
		return false;
	}
}

void TauWebAuthHelper::StopProcess() {
	if (HttpRouter)
	{
		HttpRouter->UnbindRoute(AuthHandle);
		// HttpRouter.Reset();
		// AuthHandle = nullptr;
	}
	if (FHttpServerModule::IsAvailable())
	{
		FHttpServerModule::Get().StopAllListeners();
	}
}

bool TauWebAuthHelper::GetHttpRouter() {
	if (HttpRouter.IsValid()) {
		TUDebuger::DisplayLog("HttpRouter already exist");
		return true;
	}
	static uint32 Port = 16445;
	HttpRouter = FHttpServerModule::Get().GetHttpRouter(Port);
	uint32 EndPort = Port + 10;
	// 如果端口被占用，多试两次。
	while (!HttpRouter.IsValid()) {
		HttpRouter = FHttpServerModule::Get().GetHttpRouter(Port++);
		if (Port >= EndPort) {
			Port = EndPort;
			TUDebuger::ErrorLog("The login end port is occupied");
			return false;
		}
	}
	CurrentPort = Port;
#if  PLATFORM_MAC
	// Mac版第二次接回调失效，可能是兼容问题，先新开一个端口解决下bug。
	Port++;
	TUDebuger::DisplayLog("Http port add 1");
#endif
	TUDebuger::DisplayLog("HttpRouter get success");
	return true;
}

FString TauWebAuthHelper::GetRedirectUri() {
	return FString::Printf(TEXT("http://127.0.0.1:%d/%s"), CurrentPort, *WebAuthPath);
}

FString TauWebAuthHelper::GetCodeVerifier() const {
	return CodeVerifier;
}

bool TauWebAuthHelper::SetAuthHandle() {
	if (AuthHandle.IsValid()) {
		TUDebuger::DisplayLog("AuthHandle already exist");
		return true;
	}
	if (!HttpRouter.IsValid() && !GetHttpRouter()) {
		return false;
	}
	// Register a handler for /stats
	TWeakPtr<TauWebAuthHelper> WeakThisPtr(AsShared());
	AuthHandle = HttpRouter->BindRoute(FHttpPath("/" + WebAuthPath), EHttpServerRequestVerbs::VERB_GET | EHttpServerRequestVerbs::VERB_POST,
		[WeakThisPtr](const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
	{
			TUDebuger::DisplayLog("web call back success");
		auto SharedThis = WeakThisPtr.Pin();
		if (!SharedThis.IsValid()) { return false; }
			
		SharedThis->ProcessWebAuthRequest(Request, OnComplete);
			
		return true;
	});
	if(!AuthHandle.IsValid())
	{
		TUDebuger::ErrorLog("TauWebLoginHelper unable bind route: /web login");
		return false;
	}
	TUDebuger::DisplayLog("AuthHandle create success");
	FHttpServerModule::Get().StartAllListeners();
	return true;
}

void TauWebAuthHelper::ProcessWebAuthRequest(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete) {
	for (auto QueryParam : Request.QueryParams) {
		TUDebuger::DisplayLog(QueryParam.Key + ": " + QueryParam.Value);
	}
	FString WebState = Request.QueryParams.FindRef("state");
	FString WebCode = Request.QueryParams.FindRef("code");
	FString WebMode = Request.QueryParams.FindRef("mode");
	TUniquePtr<FHttpServerResponse> ResponsePtr;
	if (WebState != State || WebCode.IsEmpty()) {
		ResponsePtr = FHttpServerResponse::Error(EHttpServerResponseCodes::NotFound);
	} else {
		//code..
		if (WebMode == "gif") {
			ResponsePtr = MakeUnique<FHttpServerResponse>();
			ResponsePtr->Code = EHttpServerResponseCodes::Ok;
			ResponsePtr->Headers.Add("Content-Type", {"image/gif"});
			ResponsePtr->Headers.Add("Access-Control-Allow-Origin", {"*"});
			FString GifBase64Str = "R0lGODlhAQABAIABAAAAAP///yH5BAEAAAEALAAAAAABAAEAAAICTAEAOw==";
			ResponsePtr->Body.Append(TUCrypto::Base64Decode(GifBase64Str));
		} else if (WebMode == "redirect") {
			ResponsePtr = MakeUnique<FHttpServerResponse>();
			ResponsePtr->Code = EHttpServerResponseCodes::Moved;
			ResponsePtr->Headers.Add("Location", {FString::Printf(TEXT("open-taptap-%s://authorize"), *TULoginImpl::Get()->Config.ClientID)});
		} else {
			ResponsePtr = MakeUnique<FHttpServerResponse>();
			ResponsePtr->Code = EHttpServerResponseCodes::Ok;
			ResponsePtr->Headers.Add("Content-Type", {"text/plain"});
			ResponsePtr->Headers.Add("Access-Control-Allow-Origin", {"*"});
			ResponsePtr->Body.Append(TUCrypto::UTF8Encode(FString("ok")));
		}
		if (CallBackBlock) {
			CallBackBlock(WebCode);
		}
#if PLATFORM_MAC || PLATFORM_WINDOWS
		TUHelper::ActivateItself();
#endif
	}
	OnComplete(MoveTemp(ResponsePtr));
}

FString TauWebAuthHelper::GenerateWebAuthUrl() {
	TSharedPtr<FJsonObject> Paras = MakeShareable(new FJsonObject);
	Paras->SetStringField("client_id", TULoginImpl::Get()->Config.ClientID);
	Paras->SetStringField("response_type", "code");
	Paras->SetStringField("redirect_uri", GetRedirectUri());
	Paras->SetStringField("state", State);
	Paras->SetStringField("code_challenge", TUCrypto::UrlBase64Encode(TUOpenSSL::Sha256(TUCrypto::UTF8Encode(CodeVerifier))));
	Paras->SetStringField("code_challenge_method", "S256");
	Paras->SetStringField("scope", FString::Join(Permissions, TEXT(",")));
	Paras->SetStringField("flow", "pc_localhost");
	FString ParaStr = TUHelper::CombinParameters(Paras);
	return TULoginRegionConfig::Get()->AccountUrl() + "?" + ParaStr;
}
