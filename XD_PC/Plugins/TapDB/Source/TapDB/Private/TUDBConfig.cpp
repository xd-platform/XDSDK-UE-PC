#include "TUDBConfig.h"


TUDBConfig * TUDBConfig::Singleton = nullptr;

FString TUDBConfig::GetHost()
{
	if (Get().isCN)
	{
		return "https://e.tapdb.net/";
	} else
	{
		return "https://e.tapdb.ap-sg.tapapis.com/";
	}
}

TUDBConfig & TUDBConfig::Get()
{
	if (Singleton == nullptr)
	{
		Singleton = new TUDBConfig;
	}
	return *Singleton;
}
