#include "TUHttpServer.h"

#include "HttpServerModule.h"
#include "TUDebuger.h"
TSharedPtr<TUHttpServer> TUHttpServer::InstancePtr = nullptr;

const TSharedPtr<TUHttpServer> TUHttpServer::Get() {
	if (InstancePtr == nullptr) {
		InstancePtr = MakeShareable(new TUHttpServer);
		InstancePtr->StartHttpServer();
	}
	return InstancePtr;
}

FString TUHttpServer::RegisterNewRoute(const FString& Path,
	TFunction<bool(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)> CallBack) {
	auto Server = Get();
	if (!Server->HttpRouter.IsValid() && Server->StartHttpServer()) {
		TUDebuger::DisplayLog("Start Http Server Fail");
		return "";
	}
	if (Server->RegisteredRoutes.Find(Path)) {
		Server->HttpRouter->UnbindRoute(Server->RegisteredRoutes[Path]);
		Server->RegisteredRoutes.Remove(Path);
	}
	// CallBack为空的话，相当于取消回调。
	if (!CallBack) {
		return "";
	}
	auto Handle = Server->HttpRouter->BindRoute(FHttpPath("/" + Path), EHttpServerRequestVerbs::VERB_GET | EHttpServerRequestVerbs::VERB_POST,
		[=](const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
	{
		return CallBack(Request, OnComplete);
	});
	if (!Handle.IsValid()) {
		TUDebuger::ErrorLog(FString::Printf(TEXT("unable bind route: %s"), *Path));
		return "";
	}
	Server->RegisteredRoutes.Add(Path, Handle);
	return FString::Printf(TEXT("http://127.0.0.1:%d/%s"), Server->CurrentPort, *Path) ;
}


bool TUHttpServer::StartHttpServer()
{
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
			TUDebuger::ErrorLog("The end port is occupied");
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
	FHttpServerModule::Get().StartAllListeners();
	return true;
}

void TUHttpServer::StopHttpServer() {
	if (FHttpServerModule::IsAvailable())
	{
		FHttpServerModule::Get().StopAllListeners();
	}

	if (HttpRouter)
	{
		for (auto RegisteredRoute : RegisteredRoutes) {
			HttpRouter->UnbindRoute(RegisteredRoute.Value);
		}
		RegisteredRoutes.Reset();
	}
	HttpRouter.Reset();
}
