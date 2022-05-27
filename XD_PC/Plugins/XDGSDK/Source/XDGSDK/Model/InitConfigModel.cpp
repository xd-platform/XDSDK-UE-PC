#include "InitConfigModel.h"

#include "TUDebuger.h"
#include "XDGNet.h"
#include "XDGSDK.h"

TSharedPtr<FInitConfigModel> FInitConfigModel::CurrentModel = nullptr;

void FInitConfigModel::SaveToLocal()
{
	if (this != CurrentModel.Get())
	{
		CurrentModel = MakeShareable(new FInitConfigModel(*this));
	}
	TUDataStorage<FXDGStorage>::SaveStruct(FXDGStorage::InitConfig, *this, true);
	if (CanShowPrivacyAlert())
	{
		SavePrivacyTxt();
	}
	
}

TSharedPtr<FInitConfigModel>& FInitConfigModel::GetLocalModel()
{
	if (CurrentModel == nullptr)
	{
		CurrentModel = TUDataStorage<FXDGStorage>::LoadStruct<FInitConfigModel>(FXDGStorage::InitConfig);
	}
	return  CurrentModel;
}

bool FInitConfigModel::CanShowPrivacyAlert()
{
	auto md = GetLocalModel();
	if (!md.IsValid())
	{
		TUDebuger::WarningLog(TEXT("请先初始化"));
		return false;
	}
	auto preStr = TUDataStorage<FXDGStorage>::LoadString(FXDGStorage::PrivacyKey);
	FString currentStr = FString::Printf(TEXT("%s-%s-%s"), *md->version, *md->configs.serviceAgreementUrl, *md->configs.serviceTermsUrl);
	if (preStr == currentStr)
	{
		return false;
	}
	return true;
}

void FInitConfigModel::UpdatePrivacyState()
{
	auto md = GetLocalModel();
	if (md.IsValid())
	{
		FString currentStr = FString::Printf(TEXT("%s-%s-%s"), *md->version, *md->configs.serviceAgreementUrl, *md->configs.serviceTermsUrl);
		TUDataStorage<FXDGStorage>::SaveString(FXDGStorage::PrivacyKey, currentStr);
	}
}

void FInitConfigModel::GetPrivacyTxt(const FString& txtUrl, TFunction<void(FString txt)> callback)
{
	FString *txt = nullptr;
	if (CurrentModel.IsValid())
	{
		txt = CurrentModel->Cache.Find(txtUrl);
	}
	if (txt == nullptr)
	{
		XDGNet::RequestPrivacyTxt(txtUrl,
		[=](FString content)
		{
			if (CurrentModel.IsValid())
			{
				CurrentModel->Cache.Add(txtUrl, content);
			}
			if (callback)
			{
				callback(content);
			}
		});
	} else
	{
		callback(*txt);
	}
}

void FInitConfigModel::SavePrivacyTxt()
{
	GetPrivacyTxt(configs.serviceTermsTxt, nullptr);
	GetPrivacyTxt(configs.serviceAgreementTxt, nullptr);
}
