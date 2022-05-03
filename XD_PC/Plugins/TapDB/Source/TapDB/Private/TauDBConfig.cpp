#include "TauDBConfig.h"


TauDBConfig * TauDBConfig::Singleton = nullptr;

FString TauDBConfig::GetHost()
{
	if (Get().isCN)
	{
		return "https://e.tapdb.net/";
	} else
	{
		return "https://e.tapdb.ap-sg.tapapis.com/";
	}
}

TauDBConfig & TauDBConfig::Get()
{
	if (Singleton == nullptr)
	{
		Singleton = new TauDBConfig;
	}
	return *Singleton;
}
