#pragma once
#include "TapAccessToken.h"
#include "TapError.h"


class TAPLOGIN_API TapAuthResult
{
public:
	enum Type
	{
		Success,
		Cancel,
		Fail,
	};
	
	static TapAuthResult CancelInit();
	static TapAuthResult SuccessInit(const TSharedPtr<FTapAccessToken>& Token);
	static TapAuthResult FailInit(const FTapError& Error);

	Type GetType();
	TSharedPtr<FTapAccessToken> GetToken();
	TSharedPtr<FTapError> GetError();

private:
	Type Type = Cancel;
	TSharedPtr<FTapAccessToken> Token = nullptr;
	TSharedPtr<FTapError> Error = nullptr;
	
	TapAuthResult();
	
};
