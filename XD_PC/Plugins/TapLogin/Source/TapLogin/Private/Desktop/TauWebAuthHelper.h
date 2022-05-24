#pragma once
#include "IHttpRouter.h"

class TauWebAuthHelper: public TSharedFromThis<TauWebAuthHelper> {
public:
	TauWebAuthHelper();
	~TauWebAuthHelper();

	bool ProcessWebAuth(const TArray<FString>& NeedPermissions, TFunction<void(FString WebCode)> CallBack);

	void StopProcess();

private:

	int32 CurrentPort = 0;

	FString State;

	TArray<FString> Permissions;

	FString CodeVerifier;
	
	bool GetHttpRouter();
public:
	FString GetRedirectUri();
	FString GetCodeVerifier() const;
private:
	bool SetAuthHandle();

	TFunction<void(FString WebCode)> CallBackBlock;
	
	TSharedPtr<IHttpRouter> HttpRouter = nullptr;

	FHttpRouteHandle AuthHandle = nullptr;

	void ProcessWebAuthRequest(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete);

	FString GenerateWebAuthUrl();

};
