#include "TapAccessToken.h"
#include "TUDataStorage.h"
#include "TULoginStorage.h"


void FTapAccessToken::SaveToLocal() const
{
	TUDataStorage<FTULoginStorage>::SaveStruct(FTULoginStorage::AccessToken, *this, true);
}

TSharedPtr<FTapAccessToken> FTapAccessToken::GetLocalModel()
{
	return  TUDataStorage<FTULoginStorage>::LoadStruct<FTapAccessToken>(FTULoginStorage::AccessToken);
}

void FTapAccessToken::ClearLocalModel()
{
	TUDataStorage<FTULoginStorage>::Remove(FTULoginStorage::AccessToken);
}
