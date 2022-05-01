#include "XDGUser.h"
#include "XDGStorage.h"

TSharedPtr<FXDGUser> FXDGUser::CurrentModel = nullptr;

void FXDGUser::SaveToLocal()
{
	if (this != CurrentModel.Get())
	{
		CurrentModel = MakeShareable(new FXDGUser(*this));
	}
	DataStorage<FXDGStorage>::SaveStruct(FXDGStorage::UserInfo, *this, true);
	
}

TSharedPtr<FXDGUser>& FXDGUser::GetLocalModel()
{
	if (CurrentModel == nullptr)
	{
		CurrentModel = DataStorage<FXDGStorage>::LoadStruct<FXDGUser>(FXDGStorage::UserInfo);
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
	DataStorage<FXDGStorage>::Remove(FXDGStorage::UserInfo);
	FSyncTokenModel::ClearToken();
	FTokenModel::ClearToken();
}

bool FXDGUser::IsPushServiceEnable()
{
	auto user = FXDGUser::GetLocalModel();
	if (user.IsValid())
	{
		FString key = user->userId + "_push_key";
		return DataStorage<FXDGStorage>::LoadBool(key);
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
		DataStorage<FXDGStorage>::SaveBool(key, enable);
	}
}




