#pragma once

class TAPLOGIN_API Region
{
public:
	virtual ~Region() = default;
	virtual FString WebHost() = 0;
	virtual FString ApiHost() = 0;
	virtual FString AccountHost() = 0;

	FString CodeUrl();
	FString TokenUrl();
	FString ProfileUrl();
	FString AccountUrl();
	
};


class TAPLOGIN_API RegionCN: Region
{
public:
	virtual FString WebHost();
	virtual FString ApiHost();
	virtual FString AccountHost();
};

class TAPLOGIN_API RegionIO: Region
{
public:
	virtual FString WebHost();
	virtual FString ApiHost();
	virtual FString AccountHost();
};
