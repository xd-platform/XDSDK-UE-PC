#include "TapAccessToken.h"
#include "DataStorage.h"
#include "TAULoginStorage.h"


void FTapAccessToken::SaveToLocal() const
{
	DataStorage<FTAULoginStorage>::SaveStruct(FTAULoginStorage::AccessToken, *this, true);
}

TSharedPtr<FTapAccessToken> FTapAccessToken::GetLocalModel()
{
	return  DataStorage<FTAULoginStorage>::LoadStruct<FTapAccessToken>(FTAULoginStorage::AccessToken);
}

void FTapAccessToken::ClearLocalModel()
{
	DataStorage<FTAULoginStorage>::Remove(FTAULoginStorage::AccessToken);
}
