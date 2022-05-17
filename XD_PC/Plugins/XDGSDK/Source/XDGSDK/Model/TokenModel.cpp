#include "TokenModel.h"
#include "XDGStorage.h"


void FTokenModel::SaveToLocal()
{
	TUDataStorage<FXDGStorage>::SaveStruct(FXDGStorage::TokenInfo, *this, true);
}

TSharedPtr<FTokenModel> FTokenModel::GetLocalModel()
{
	return  TUDataStorage<FXDGStorage>::LoadStruct<FTokenModel>(FXDGStorage::TokenInfo);
}

void FTokenModel::ClearToken()
{
	TUDataStorage<FXDGStorage>::Remove(FXDGStorage::TokenInfo);
}



void FSyncTokenModel::SaveToLocal()
{
	TUDataStorage<FXDGStorage>::SaveStruct(FXDGStorage::SessionTokenKey, *this, true);
}

TSharedPtr<FSyncTokenModel> FSyncTokenModel::GetLocalModel()
{
	return  TUDataStorage<FXDGStorage>::LoadStruct<FSyncTokenModel>(FXDGStorage::SessionTokenKey);
}

void FSyncTokenModel::ClearToken()
{
	TUDataStorage<FXDGStorage>::Remove(FXDGStorage::SessionTokenKey);
}
