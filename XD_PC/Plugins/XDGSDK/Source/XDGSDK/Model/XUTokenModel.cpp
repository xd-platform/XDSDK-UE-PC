#include "XUTokenModel.h"
#include "XDGStorage.h"


void FXUTokenModel::SaveToLocal()
{
	TUDataStorage<FXDGStorage>::SaveStruct(FXDGStorage::TokenInfo, *this, true);
}

TSharedPtr<FXUTokenModel> FXUTokenModel::GetLocalModel()
{
	return  TUDataStorage<FXDGStorage>::LoadStruct<FXUTokenModel>(FXDGStorage::TokenInfo);
}

void FXUTokenModel::ClearToken()
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
