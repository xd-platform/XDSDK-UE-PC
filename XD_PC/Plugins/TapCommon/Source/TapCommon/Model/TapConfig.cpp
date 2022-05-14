#include "TapConfig.h"

FTapConfig * FTapConfig::SharedConfig = nullptr;


FTapConfig & FTapConfig::Get()
{
	if (SharedConfig == nullptr)
	{
		SharedConfig = new FTapConfig;
	}
	return *SharedConfig;
}