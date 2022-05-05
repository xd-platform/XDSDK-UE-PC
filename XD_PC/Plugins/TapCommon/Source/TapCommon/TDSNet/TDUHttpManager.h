#pragma once
#include "TDUHttpRequest.h"

class TAPCOMMON_API TDUHttpManager
{
public:
	static TDUHttpManager& Get();

	void request(TSharedPtr<TDUHttpRequest> tdsReq);

private:
	static TDUHttpManager* Singleton;
};
