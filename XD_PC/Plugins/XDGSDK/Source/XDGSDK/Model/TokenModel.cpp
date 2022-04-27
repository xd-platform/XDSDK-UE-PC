#include "TokenModel.h"
#include "XDGDataStorageName.h"


void FTokenModel::SaveToLocal()
{
	DataStorage::SaveStruct(XDGDataStorageName::TokenInfo, *this, true);
}

TSharedPtr<FTokenModel> FTokenModel::GetLocalModel()
{
	return  DataStorage::LoadStruct<FTokenModel>(XDGDataStorageName::TokenInfo);
}

void FTokenModel::ClearToken()
{
	DataStorage::Remove(XDGDataStorageName::TokenInfo);
}



void FSyncTokenModel::SaveToLocal()
{
	DataStorage::SaveStruct(XDGDataStorageName::SessionTokenKey, *this, true);
}

TSharedPtr<FSyncTokenModel> FSyncTokenModel::GetLocalModel()
{
	return  DataStorage::LoadStruct<FSyncTokenModel>(XDGDataStorageName::SessionTokenKey);
}

void FSyncTokenModel::ClearToken()
{
	DataStorage::Remove(XDGDataStorageName::SessionTokenKey);
}
