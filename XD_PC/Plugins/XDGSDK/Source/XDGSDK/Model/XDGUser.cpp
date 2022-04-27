#include "XDGUser.h"
#include "XDGDataStorageName.h"

TSharedPtr<FXDGUser> FXDGUser::CurrentModel = nullptr;

void FXDGUser::SaveToLocal()
{
	if (this != CurrentModel.Get())
	{
		CurrentModel = MakeShareable(new FXDGUser(*this));
	}
	DataStorage::SaveStruct(XDGDataStorageName::UserInfo, *this, true);
	
}

TSharedPtr<FXDGUser>& FXDGUser::GetLocalModel()
{
	if (CurrentModel == nullptr)
	{
		CurrentModel = DataStorage::LoadStruct<FXDGUser>(XDGDataStorageName::UserInfo);
	}
	return  CurrentModel;
}

TSharedPtr<FTokenModel> FXDGUser::Token()
{
	return  FTokenModel::GetLocalModel();
}

LoginType FXDGUser::GetLoginType()
{
	if (loginType == 0){
		return LoginType::Guest;
	} else if (loginType == 5){
		return LoginType::TapTap;
	}
	return LoginType::Guest;
}

void FXDGUser::ClearUserData()
{
	CurrentModel = nullptr;
	DataStorage::Remove(XDGDataStorageName::UserInfo);
	FSyncTokenModel::ClearToken();
	FTokenModel::ClearToken();
}

bool FXDGUser::IsPushServiceEnable()
{
	auto user = FXDGUser::GetLocalModel();
	if (user.IsValid())
	{
		FString key = user->userId + "_push_key";
		return DataStorage::LoadBool(key);
	} else
	{
		return false;
	}
}

void FXDGUser::SetPushServiceEnable(bool enable)
{
	auto user = FXDGUser::GetLocalModel();
	if (user.IsValid())
	{
		FString key = user->userId + "_push_key";
		DataStorage::SaveBool(key, enable);
	}
}




