#include "TokenModel.h"
#include "XDGStorage.h"


void FTokenModel::SaveToLocal()
{
	DataStorage<FXDGStorage>::SaveStruct(FXDGStorage::TokenInfo, *this, true);
}

TSharedPtr<FTokenModel> FTokenModel::GetLocalModel()
{
	return  DataStorage<FXDGStorage>::LoadStruct<FTokenModel>(FXDGStorage::TokenInfo);
}

void FTokenModel::ClearToken()
{
	DataStorage<FXDGStorage>::Remove(FXDGStorage::TokenInfo);
}



void FSyncTokenModel::SaveToLocal()
{
	DataStorage<FXDGStorage>::SaveStruct(FXDGStorage::SessionTokenKey, *this, true);
}

TSharedPtr<FSyncTokenModel> FSyncTokenModel::GetLocalModel()
{
	return  DataStorage<FXDGStorage>::LoadStruct<FSyncTokenModel>(FXDGStorage::SessionTokenKey);
}

void FSyncTokenModel::ClearToken()
{
	DataStorage<FXDGStorage>::Remove(FXDGStorage::SessionTokenKey);
}
