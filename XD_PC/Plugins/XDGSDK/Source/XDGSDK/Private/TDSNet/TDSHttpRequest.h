#pragma once
#include "CoreMinimal.h"
#include "TDSHttpResponse.h"
#include "TDSHttpObject.h"

class TDSHttpRequest
{
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
	
	Type type = Get;

	typedef void (*CompletedBlock)(TSharedPtr<TDSHttpResponse> response);
	CompletedBlock onCompleted = nullptr;
	
	virtual TMap<FString, FString> commonHeaders(){ return TMap<FString, FString>(); };
};
