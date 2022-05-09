#pragma once
#include "IHttpRouter.h"

class TauWebAuthHelper: public TSharedFromThis<TauWebAuthHelper> {
public:
	TauWebAuthHelper(TArray<FString> Permissions);
	~TauWebAuthHelper();

	bool ProcessWebAuth();

private:

	int32 CurrentPort = 0;

	FString State;

	TArray<FString> Permissions;

	FString CodeChallenge;
	
	bool GetHttpRouter();
	bool SetAuthHandle();

	
	
	TSharedPtr<IHttpRouter> HttpRouter = nullptr;

	FHttpRouteHandle AuthHandle = nullptr;

	void ProcessWebAuthRequest(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete);

	FString GenerateWebAuthUrl();

};
