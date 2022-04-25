#pragma once
#include "TDUHttpRequest.h"

class TAPCOMMON_API TDUHttpManager
{
public:
	static TDUHttpManager& Get();

	void request(TSharedPtr<TDUHttpRequest> tdsReq);

	static FString CombinParameters(TSharedPtr<FJsonObject> parameters, bool isEncode = true);
private:
	static TDUHttpManager* Singleton;
};
