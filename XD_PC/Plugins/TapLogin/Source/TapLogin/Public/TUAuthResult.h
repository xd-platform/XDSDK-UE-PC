#pragma once
#include "TUAccessToken.h"
#include "TUError.h"


class TAPLOGIN_API TUAuthResult
{
public:
	enum Type
	{
		Success,
		Cancel,
		Fail,
	};
	
	static TUAuthResult CancelInit();
	static TUAuthResult SuccessInit(const TSharedPtr<FTUAccessToken>& Token);
	static TUAuthResult FailInit(const FTUError& Error);

	Type GetType();
	TSharedPtr<FTUAccessToken> GetToken();
	TSharedPtr<FTUError> GetError();

private:
	Type Type = Cancel;
	TSharedPtr<FTUAccessToken> Token = nullptr;
	TSharedPtr<FTUError> Error = nullptr;
	
	TUAuthResult();
};
