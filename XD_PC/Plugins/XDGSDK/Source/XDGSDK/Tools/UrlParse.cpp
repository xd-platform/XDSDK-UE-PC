#include "UrlParse.h"

/* 最终目标，暂时简单一点
https://johnny:p4ssw0rd@www.example.com:443/script.ext;param=value?query=value#ref

Component						Value
scheme							https
user							johnny
password						p4ssw0rd
host							www.example.com
port							443
path							/script.ext
pathExtension					ext
pathComponents					["/", "script.ext"]
parameterString					param=value
query							query=value
fragment						ref
*/ 
UrlParse::UrlParse(FString url)
{
	IsVaild = false;
	if (url.Len() <= 0)
	{
		return;
	}
	FString otherStr = url;
	FString searchStr = "://";
	auto tempIndex= url.Find(searchStr);
	if (tempIndex == INDEX_NONE)
	{
		return;
	}
	Scheme = otherStr.Left(tempIndex);
	otherStr.RightChopInline(tempIndex + searchStr.Len());
	// Host = otherStr;

	if (Scheme == "http")
	{
		Port = "80";
	} else if (Scheme == "https")
	{
		Port = "443";
	}

	IsVaild = true;

	// 找host
	searchStr = "/";
	tempIndex = otherStr.Find(searchStr);
	if (tempIndex == INDEX_NONE)
	{
		Host = otherStr;
		otherStr = "";
	} else
	{
		Host = otherStr.Left(tempIndex);
		otherStr.RightChopInline(tempIndex + searchStr.Len());
	}

	// 看看host中有没有port
	searchStr = ":";
	tempIndex = Host.Find(searchStr);
	if (tempIndex != INDEX_NONE)
	{
		Port = Host.RightChop(tempIndex + searchStr.Len());
		Host.LeftInline(tempIndex);
	}

	if (otherStr.Len() <= 0)
	{
		return;
	}

	// 找path query
	searchStr = "?";
	tempIndex = otherStr.Find(searchStr);
	if (tempIndex == INDEX_NONE)
	{
		Path = otherStr;
		otherStr = "";
	} else
	{
		Path = otherStr.Left(tempIndex);
		query = otherStr.RightChop(tempIndex + searchStr.Len());
	}

	
}
