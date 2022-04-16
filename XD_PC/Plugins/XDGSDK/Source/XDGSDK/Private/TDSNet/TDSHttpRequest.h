#pragma once
#include "CoreMinimal.h"
#include "TDSHttpResponse.h"

class TDSHttpRequest
{
	friend class TDSHttpManager;
public:
	virtual ~TDSHttpRequest() = default;

	enum Type
	{
		Get,
		Post,
	};
	
	FString URL;
	
	TMap<FString, FString> headers;

	FJsonObject paras;

	int repeatCount = 1;
	
	Type type = Get;

	DECLARE_DELEGATE_OneParam(CompletedBlock, TSharedPtr<TDSHttpResponse>);
	CompletedBlock onCompleted;
	
	virtual TMap<FString, FString> commonHeaders(){ return TMap<FString, FString>(); };

private:
	int tryCount = 0;
};
