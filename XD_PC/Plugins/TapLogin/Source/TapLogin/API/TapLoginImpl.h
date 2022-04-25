#pragma once
#include "TapAuthResult.h"

class TapLoginImpl
{
public:
	static void Init(const FString& clientID, bool isCn, bool roundCorner);

	static TapLoginImpl& Get();

	static void Login(TArray<FString> Permissions, TFunction<void(TapAuthResult)> CallBack);


private:
	static TapLoginImpl* Singleton;
};
