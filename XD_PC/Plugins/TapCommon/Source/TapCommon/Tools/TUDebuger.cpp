#include "TUDebuger.h"

DEFINE_LOG_CATEGORY_STATIC(TapLog, Log, All);

bool TUDebuger::IsTest = false;
TMap<FString, FString> TUDebuger::ReplaceHosts;

void TUDebuger::DisplayLog(const FString& Info)
{
	UE_LOG(TapLog, Display, TEXT("%s"), *Info);
}

void TUDebuger::WarningLog(const FString& Info)
{
	UE_LOG(TapLog, Warning, TEXT("%s"), *Info);
}

void TUDebuger::ErrorLog(const FString& Info)
{
	UE_LOG(TapLog, Error, TEXT("%s"), *Info);
}

void TUDebuger::DisplayShow(const FString& Info, float Duration)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Green, Info, true, FVector2D(2, 2));
	}
}

void TUDebuger::WarningShow(const FString& Info, float Duration)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Yellow, Info, true, FVector2D(2, 2));
	}
}

void TUDebuger::ErrorShow(const FString& Info, float Duration)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Red, Info, true, FVector2D(2, 2));
	}
}


