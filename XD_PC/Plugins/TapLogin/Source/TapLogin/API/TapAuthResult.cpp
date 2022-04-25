#include "TapAuthResult.h"

TSharedPtr<TapAuthResult> TapAuthResult::CancelInit()
{
	return MakeShareable(new TapAuthResult);
}

TSharedPtr<TapAuthResult> TapAuthResult::SuccessInit(const TSharedPtr<FTapAccessToken>& Token)
{
	TSharedPtr<TapAuthResult> Result = MakeShareable(new TapAuthResult);
	Result->Type = Success;
	Result->Token = Token;
	return Result;
}

TSharedPtr<TapAuthResult> TapAuthResult::FailInit(const FTapError& Error)
{
	TSharedPtr<TapAuthResult> Result = MakeShareable(new TapAuthResult);
	Result->Type = Fail;
	Result->Error = MakeShareable(new FTapError(Error));
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

