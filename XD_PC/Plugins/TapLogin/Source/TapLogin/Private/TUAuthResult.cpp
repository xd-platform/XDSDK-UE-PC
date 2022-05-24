#include "TUAuthResult.h"

TUAuthResult TUAuthResult::CancelInit()
{
	return TUAuthResult();
}

TUAuthResult TUAuthResult::SuccessInit(const TSharedPtr<FTUAccessToken>& Token)
{
	TUAuthResult Result;
	Result.Type = Success;
	Result.Token = Token;
	return Result;
}

TUAuthResult TUAuthResult::FailInit(const FTUError& Error)
{
	TUAuthResult Result;
	Result.Type = Fail;
	Result.Error = MakeShareable(new FTUError(Error));
	return Result;
}

enum TUAuthResult::Type TUAuthResult::GetType()
{
	return Type;
}

TSharedPtr<FTUAccessToken> TUAuthResult::GetToken()
{
	return Token;
}

TSharedPtr<FTUError> TUAuthResult::GetError()
{
	return Error;
}

TUAuthResult::TUAuthResult()
{
	Type = Cancel;
	Token = nullptr;
	Error = nullptr;
}

