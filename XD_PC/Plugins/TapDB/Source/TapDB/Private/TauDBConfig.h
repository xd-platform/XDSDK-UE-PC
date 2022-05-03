#pragma once

class TauDBConfig
{
public:

	bool isCN = true;
	static TauDBConfig& Get();
	static FString GetHost();

	
private:
	static TauDBConfig *Singleton;
	
};
