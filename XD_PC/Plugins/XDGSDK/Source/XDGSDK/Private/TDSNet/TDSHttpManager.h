#pragma once
#include "TDSHttpRequest.h"

class TDSHttpManager
{
public:
	static TDSHttpManager& Get();

	void request(TSharedPtr<TDSHttpRequest> tdsReq);

	static FString CombinParameters(TSharedPtr<FJsonObject> parameters);
private:
	static TDSHttpManager* Singleton;
};
