#include "TAUDBConfig.h"


TAUDBConfig * TAUDBConfig::Singleton = nullptr;

FString TAUDBConfig::GetHost()
{
	if (Get().isCN)
	{
		return "https://e.tapdb.net/";
	} else
	{
		return "https://e.tapdb.ap-sg.tapapis.com/";
	}
}

TAUDBConfig & TAUDBConfig::Get()
{
	if (Singleton == nullptr)
	{
		Singleton = new TAUDBConfig;
	}
	return *Singleton;
}
