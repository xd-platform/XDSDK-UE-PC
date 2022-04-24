#include "TDSHelper.h"

void TDSHelper::Debug(FString Message, float Duration)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Yellow, Message);
	}
}
