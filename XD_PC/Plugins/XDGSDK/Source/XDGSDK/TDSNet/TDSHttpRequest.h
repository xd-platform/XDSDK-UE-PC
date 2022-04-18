#pragma once
#include "CoreMinimal.h"
#include "TDSHttpResponse.h"

class TDSHttpRequest
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
	
	TMap<FString, FString> headers;

	TSharedPtr<FJsonObject> parameters;

	int repeatCount = 1;

	float timeoutSecs = 10;
	
	Type type = Get;

	DECLARE_DELEGATE_OneParam(CompletedBlock, TSharedPtr<TDSHttpResponse>);
	CompletedBlock onCompleted;

	// 请求的时候，是否不加入CommonHeaders和CommonParameters。
	bool isPure = false; 
	virtual TMap<FString, FString> CommonHeaders(){ return TMap<FString, FString>(); };
	virtual TSharedPtr<FJsonObject> CommonParameters(){ return nullptr; };

private:
	int tryCount = 0;
};
