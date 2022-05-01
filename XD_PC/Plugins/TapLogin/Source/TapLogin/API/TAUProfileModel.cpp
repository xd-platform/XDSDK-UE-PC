#include "TAUProfileModel.h"
#include "DataStorage.h"
#include "TAULoginStorage.h"

void FTAUProfileModel::SaveToLocal()
{
	DataStorage<FTAULoginStorage>::SaveStruct(FTAULoginStorage::Profile, *this, true);
}
	
TSharedPtr<FTAUProfileModel> FTAUProfileModel::GetLocalModel()
{
	return  DataStorage<FTAULoginStorage>::LoadStruct<FTAUProfileModel>(FTAULoginStorage::Profile);
}

void FTAUProfileModel::ClearLocalModel()
{
	DataStorage<FTAULoginStorage>::Remove(FTAULoginStorage::Profile);
}
