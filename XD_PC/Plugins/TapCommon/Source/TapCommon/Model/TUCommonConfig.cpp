#include "TUCommonConfig.h"

FTUCommonConfig * FTUCommonConfig::SharedConfig = nullptr;


FTUCommonConfig & FTUCommonConfig::Get()
{
	if (SharedConfig == nullptr)
	{
		SharedConfig = new FTUCommonConfig;
	}
	return *SharedConfig;
}