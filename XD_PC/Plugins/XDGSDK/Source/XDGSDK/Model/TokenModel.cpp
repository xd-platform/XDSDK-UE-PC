
#include "TokenModel.h"

#include "DataStorageName.h"

TSharedPtr<FTokenModel> FTokenModel::currentToken = nullptr;

TSharedPtr<FTokenModel> FTokenModel::GetCurrentToken()
{
	if (currentToken == nullptr)
	{
		currentToken = DataStorage::LoadStruct<FTokenModel>(DataStorageName::TokenInfo);
	}
	return currentToken;
}

void FTokenModel::SetCurrentToken(TSharedPtr<FTokenModel> model)
{
	currentToken = model;
	DataStorage::SaveStruct(DataStorageName::TokenInfo, model);
}

