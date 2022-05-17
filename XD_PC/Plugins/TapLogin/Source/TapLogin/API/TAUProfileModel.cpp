#include "TAUProfileModel.h"
#include "TUDataStorage.h"
#include "TULoginStorage.h"

void FTAUProfileModel::SaveToLocal()
{
	TUDataStorage<FTULoginStorage>::SaveStruct(FTULoginStorage::Profile, *this, true);
}
	
TSharedPtr<FTAUProfileModel> FTAUProfileModel::GetLocalModel()
{
	return  TUDataStorage<FTULoginStorage>::LoadStruct<FTAUProfileModel>(FTULoginStorage::Profile);
}

void FTAUProfileModel::ClearLocalModel()
{
	TUDataStorage<FTULoginStorage>::Remove(FTULoginStorage::Profile);
}
