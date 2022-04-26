#include "TapAuthResult.h"

TapAuthResult TapAuthResult::CancelInit()
{
	return TapAuthResult();
}

TapAuthResult TapAuthResult::SuccessInit(const TSharedPtr<FTapAccessToken>& Token)
{
	TapAuthResult Result;
	Result.Type = Success;
	Result.Token = Token;
	return Result;
}

TapAuthResult TapAuthResult::FailInit(const FTapError& Error)
{
	TapAuthResult Result;
	Result.Type = Fail;
	Result.Error = MakeShareable(new FTapError(Error));
	return Result;
}

enum TapAuthResult::Type TapAuthResult::GetType()
{
	return Type;
}

TSharedPtr<FTapAccessToken> TapAuthResult::GetToken()
{
	return Token;
}

TSharedPtr<FTapError> TapAuthResult::GetError()
{
	return Error;
}

TapAuthResult::TapAuthResult()
{
	Type = Cancel;
	Token = nullptr;
	Error = nullptr;
}

