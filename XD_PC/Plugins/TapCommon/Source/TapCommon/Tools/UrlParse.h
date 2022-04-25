#pragma once

class TAPCOMMON_API UrlParse
{
public:
	UrlParse(FString url);

	bool IsVaild;

	FString Scheme;
	FString Host;
	FString Path;
	FString Port;
	FString query;
	
};
