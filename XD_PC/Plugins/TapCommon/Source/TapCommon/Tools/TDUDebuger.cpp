#include "TDUDebuger.h"

DEFINE_LOG_CATEGORY_STATIC(TapLog, Log, All);

void TDUDebuger::DisplayLog(const FString& Info)
{
	UE_LOG(TapLog, Display, TEXT("%s"), *Info);
}

void TDUDebuger::WarningLog(const FString& Info)
{
	UE_LOG(TapLog, Warning, TEXT("%s"), *Info);
}

void TDUDebuger::ErrorLog(const FString& Info)
{
	UE_LOG(TapLog, Error, TEXT("%s"), *Info);
}

void TDUDebuger::DisplayShow(const FString& Info, float Duration)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Green, Info);
	}
}

void TDUDebuger::WarningShow(const FString& Info, float Duration)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Yellow, Info);
	}
}

void TDUDebuger::ErrorShow(const FString& Info, float Duration)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Red, Info);
	}
}


