#include "TUAccessToken.h"
#include "TUDataStorage.h"
#include "TULoginStorage.h"


void FTUAccessToken::SaveToLocal() const
{
	TUDataStorage<FTULoginStorage>::SaveStruct(FTULoginStorage::AccessToken, *this, true);
}

TSharedPtr<FTUAccessToken> FTUAccessToken::GetLocalModel()
{
	return  TUDataStorage<FTULoginStorage>::LoadStruct<FTUAccessToken>(FTULoginStorage::AccessToken);
}

void FTUAccessToken::ClearLocalModel()
{
	TUDataStorage<FTULoginStorage>::Remove(FTULoginStorage::AccessToken);
}
