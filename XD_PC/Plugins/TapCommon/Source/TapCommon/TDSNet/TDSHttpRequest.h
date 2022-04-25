#pragma once
#include "CoreMinimal.h"
#include "TDSHttpResponse.h"

class TAPCOMMON_API TDSHttpRequest
{
	friend class TDSHttpManager;
public:
	TDSHttpRequest();
	virtual ~TDSHttpRequest() = default;
	// virtual ~TDSHttpRequest();

	enum Type
	{
		Get,
		Post,
	};
	
	FString URL;
	
	TMap<FString, FString> Headers;

	// 参数，get请求会把参数拼接到url后面，post请求会放在body里。
	TSharedPtr<FJsonObject> Parameters;

	// post请求把参数拼在url后面，明明post请求可以把参数放在body里的。。特殊改的需求
	TSharedPtr<FJsonObject> PostUrlParameters;

	int RepeatCount = 1;

	float TimeoutSecs = 10;
	
	Type type = Get;

	DECLARE_DELEGATE_OneParam(CompletedBlock, TSharedPtr<TDSHttpResponse>);
	CompletedBlock onCompleted;

	FString GetFinalUrl();

	// 请求的时候，是否不加入CommonHeaders和CommonParameters。
	bool isPure = false; 
	virtual TMap<FString, FString> CommonHeaders(){ return TMap<FString, FString>(); };
	virtual TSharedPtr<FJsonObject> CommonParameters(){ return nullptr; };

	// virtual void DoSomeingAfterCombinHeadersAndParas(){  };
	// 真正的request已经配置完，这里给个时机重新设置一下
	virtual bool ResetHeadersBeforeRequest(){ return false; };
	// virtual bool ResetParametersBeforeRequest(){ return false; };

private:
	int tryCount = 0;
	FString FinalURL;
};
