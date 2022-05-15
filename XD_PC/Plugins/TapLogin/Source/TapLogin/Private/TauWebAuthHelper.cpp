#include "TauWebAuthHelper.h"

#include "HttpServerModule.h"
#include "HttpServerResponse.h"
#include "TapTapSdk.h"
#include "TDSCrypto.h"
#include "TDSHelper.h"
#include "TDUDebuger.h"
#include "TduOpenSSL.h"

static FString WebAuthPath = "authorize";

TauWebAuthHelper::TauWebAuthHelper() {
	State = FGuid::NewGuid().ToString();
	CodeVerifier = TDSHelper::GetRandomStr(16);
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
		TDUDebuger::DisplayLog("HttpRouter already exist");
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
			TDUDebuger::ErrorLog("The login end port is occupied");
			return false;
		}
	}
	CurrentPort = Port;
#if  PLATFORM_MAC
	// Mac版第二次接回调失效，可能是兼容问题，先新开一个端口解决下bug。
	Port++;
	TDUDebuger::DisplayLog("Http port add 1");
#endif
	TDUDebuger::DisplayLog("HttpRouter get success");
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
		TDUDebuger::DisplayLog("AuthHandle already exist");
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
			TDUDebuger::DisplayLog("web call back success");
		auto SharedThis = WeakThisPtr.Pin();
		if (!SharedThis.IsValid()) { return false; }
			
		SharedThis->ProcessWebAuthRequest(Request, OnComplete);
			
		return true;
	});
	if(!AuthHandle.IsValid())
	{
		TDUDebuger::ErrorLog("TauWebLoginHelper unable bind route: /web login");
		return false;
	}
	TDUDebuger::DisplayLog("AuthHandle create success");
	FHttpServerModule::Get().StartAllListeners();
	return true;
}

void TauWebAuthHelper::ProcessWebAuthRequest(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete) {
	for (auto QueryParam : Request.QueryParams) {
		TDUDebuger::DisplayLog(QueryParam.Key + ": " + QueryParam.Value);
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
			ResponsePtr->Body.Append(TDSCrypto::Base64Decode(GifBase64Str));
		} else if (WebMode == "redirect") {
			ResponsePtr = MakeUnique<FHttpServerResponse>();
			ResponsePtr->Code = EHttpServerResponseCodes::Moved;
			ResponsePtr->Headers.Add("Location", {FString::Printf(TEXT("open-taptap-%s://authorize"), *TapTapSdk::ClientId)});
		} else {
			ResponsePtr = MakeUnique<FHttpServerResponse>();
			ResponsePtr->Code = EHttpServerResponseCodes::Ok;
			ResponsePtr->Headers.Add("Content-Type", {"text/plain"});
			ResponsePtr->Headers.Add("Access-Control-Allow-Origin", {"*"});
			ResponsePtr->Body.Append(TDSCrypto::UTF8Encode(FString("ok")));
		}
		if (CallBackBlock) {
			CallBackBlock(WebCode);
		}
#if PLATFORM_MAC || PLATFORM_WINDOWS
		TDSHelper::ActivateItself();
#endif
	}
	OnComplete(MoveTemp(ResponsePtr));
}

FString TauWebAuthHelper::GenerateWebAuthUrl() {
	TSharedPtr<FJsonObject> Paras = MakeShareable(new FJsonObject);
	Paras->SetStringField("client_id", TapTapSdk::ClientId);
	Paras->SetStringField("response_type", "code");
	Paras->SetStringField("redirect_uri", GetRedirectUri());
	Paras->SetStringField("state", State);
	Paras->SetStringField("code_challenge", TDSCrypto::UrlBase64Encode(TduOpenSSL::Sha256(TDSCrypto::UTF8Encode(CodeVerifier))));
	Paras->SetStringField("code_challenge_method", "S256");
	Paras->SetStringField("scope", FString::Join(Permissions, TEXT(",")));
	Paras->SetStringField("flow", "pc_localhost");
	FString ParaStr = TDSHelper::CombinParameters(Paras);
	return TapTapSdk::CurrentRegion->AccountUrl() + "?" + ParaStr;
}
