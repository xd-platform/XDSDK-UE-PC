#include "TauWebAuthHelper.h"

#include "HttpServerModule.h"
#include "TapTapSdk.h"
#include "TDSCrypto.h"
#include "TDSHelper.h"
#include "TDUDebuger.h"
#include "TduOpenSSL.h"

static FString WebAuthPath = "authorize";

TauWebAuthHelper::TauWebAuthHelper(TArray<FString> Permissions) : Permissions(Permissions) {
	State = FGuid::NewGuid().ToString();
	CodeChallenge = TDSCrypto::UrlBase64Encode(TduOpenSSL::Sha256(TDSCrypto::UTF8Encode(TDSHelper::GetRandomStr(8))));
}

TauWebAuthHelper::~TauWebAuthHelper() {
	if (HttpRouter)
	{
		HttpRouter->UnbindRoute(AuthHandle);
	}
}

bool TauWebAuthHelper::ProcessWebAuth() {
	if (GetHttpRouter() && SetAuthHandle()) {
		FPlatformProcess::LaunchURL(*GenerateWebAuthUrl(), nullptr, nullptr);
		return true;
	} else {
		return false;
	}
}

bool TauWebAuthHelper::GetHttpRouter() {
	if (HttpRouter.IsValid()) {
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
	return true;
}

bool TauWebAuthHelper::SetAuthHandle() {
	if (AuthHandle.IsValid()) {
		return true;
	}
	if (!HttpRouter.IsValid() && GetHttpRouter()) {
		return false;
	}
	// Register a handler for /stats
	TWeakPtr<TauWebAuthHelper> WeakThisPtr(AsShared());
	AuthHandle = HttpRouter->BindRoute(FHttpPath("/" + WebAuthPath), EHttpServerRequestVerbs::VERB_GET,
		[WeakThisPtr](const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
	{
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
	return true;
}

void TauWebAuthHelper::ProcessWebAuthRequest(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete) {
	
}

FString TauWebAuthHelper::GenerateWebAuthUrl() {
	TSharedPtr<FJsonObject> Paras = MakeShareable(new FJsonObject);
	Paras->SetStringField("client_id", TapTapSdk::ClientId);
	Paras->SetStringField("response_type", "code");
	Paras->SetStringField("redirect_uri", FString::Printf(TEXT("http://127.0.0.1:%d/%s"), CurrentPort, *WebAuthPath));
	Paras->SetStringField("state", State);
	Paras->SetStringField("code_challenge", CodeChallenge);
	Paras->SetStringField("code_challenge_method", "S256");
	Paras->SetStringField("scope", FString::Join(Permissions, TEXT(",")));
	Paras->SetStringField("flow", "pc_localhost");
	FString ParaStr = TDSHelper::CombinParameters(Paras);
	return TapTapSdk::CurrentRegion->AccountUrl() + "?" + ParaStr;
}
