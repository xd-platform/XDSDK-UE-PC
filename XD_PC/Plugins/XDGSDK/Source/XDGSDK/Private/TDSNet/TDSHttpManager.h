#pragma once
#include "TDSHttpRequest.h"

class TDSHttpManager
{
public:
	static TDSHttpManager& Get();

	void request(TSharedPtr<TDSHttpRequest> tdsReq);
private:
	static TDSHttpManager* Singleton;
};
