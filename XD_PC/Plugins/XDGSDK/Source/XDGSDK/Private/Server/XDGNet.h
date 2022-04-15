#pragma once
#include "TDSNet/TDSHttpManager.h"

class XDGNet: public TDSHttpRequest
{
public:

	static void RequestConfig();

	static void RequestIpInfo();
	
};
