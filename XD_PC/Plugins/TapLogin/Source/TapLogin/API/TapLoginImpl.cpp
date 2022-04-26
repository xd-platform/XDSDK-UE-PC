#include "TapLoginImpl.h"

#include "TapTapSdk.h"

TapLoginImpl* TapLoginImpl::Singleton = NULL;

TapLoginImpl& TapLoginImpl::Get()
{
	if (Singleton == NULL)
	{
		check(IsInGameThread());
		Singleton = new TapLoginImpl();
	}
	return *Singleton;
}

void TapLoginImpl::Login(TArray<FString> Permissions, TFunction<void(TapAuthResult)> CallBack)
{
	
}

void TapLoginImpl::Init(const FString& clientID, bool isCn, bool roundCorner)
{
	TapTapSdk::SDKInitialize(clientID, isCn);
}