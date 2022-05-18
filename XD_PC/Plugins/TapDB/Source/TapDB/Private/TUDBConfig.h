#pragma once

class TUDBConfig
{
public:

	bool isCN = true;
	static TUDBConfig& Get();
	static FString GetHost();

	
private:
	static TUDBConfig *Singleton;
	
};
