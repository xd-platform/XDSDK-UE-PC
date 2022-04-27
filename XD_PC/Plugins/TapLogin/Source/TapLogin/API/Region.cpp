#include "Region.h"

FString Region::CodeUrl()
{
	return WebHost() + "/oauth2/v1/device/code";
}

FString Region::TokenUrl()
{
	return WebHost() + "/oauth2/v1/token";
}

FString Region::ProfileUrl()
{
	return ApiHost() + "/account/profile/v1";
}

FString Region::AccountUrl()
{
	return AccountHost() + "/authorize?";
}


FString RegionCN::WebHost()
{
	return "https://www.taptap.com";
}

FString RegionCN::ApiHost()
{
	return "https://openapi.taptap.com";
}

FString RegionCN::AccountHost()
{
	return "https://accounts.taptap.com";
}

FString RegionIO::WebHost()
{
	return "https://www.taptap.io";
}

FString RegionIO::ApiHost()
{
	return "https://openapi.tap.io";
}

FString RegionIO::AccountHost()
{
	return "https://accounts.taptap.io";
}