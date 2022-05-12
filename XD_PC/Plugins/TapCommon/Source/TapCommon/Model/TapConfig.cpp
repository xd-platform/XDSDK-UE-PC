#include "TapConfig.h"

FTapConfig * FTapConfig::Singleton = nullptr;


FTapConfig & FTapConfig::Get()
{
	if (Singleton == nullptr)
	{
		Singleton = new FTapConfig;
	}
	return *Singleton;
}