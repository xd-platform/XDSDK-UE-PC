#pragma once

class TAUDBConfig
{
public:

	bool isCN = true;

	static FString GetHost();

	
private:
	static TAUDBConfig *Singleton;
	static TAUDBConfig& Get();
};
