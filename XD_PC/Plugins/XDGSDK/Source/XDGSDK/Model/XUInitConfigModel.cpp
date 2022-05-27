#include "XUInitConfigModel.h"

#include "TUDebuger.h"
#include "XUNet.h"
#include "XDGSDK.h"

TSharedPtr<FXUInitConfigModel> FXUInitConfigModel::CurrentModel = nullptr;

void FXUInitConfigModel::SaveToLocal()
{
	if (this != CurrentModel.Get())
	{
		CurrentModel = MakeShareable(new FXUInitConfigModel(*this));
	}
	TUDataStorage<FXUStorage>::SaveStruct(FXUStorage::InitConfig, *this, true);
	if (CanShowPrivacyAlert())
	{
		SavePrivacyTxt();
	}
	
}

TSharedPtr<FXUInitConfigModel>& FXUInitConfigModel::GetLocalModel()
{
	if (CurrentModel == nullptr)
	{
		CurrentModel = TUDataStorage<FXUStorage>::LoadStruct<FXUInitConfigModel>(FXUStorage::InitConfig);
	}
	return  CurrentModel;
}

bool FXUInitConfigModel::CanShowPrivacyAlert()
{
	auto md = GetLocalModel();
	if (!md.IsValid())
	{
		TUDebuger::WarningLog(TEXT("请先初始化"));
		return false;
	}
	auto preStr = TUDataStorage<FXUStorage>::LoadString(FXUStorage::PrivacyKey);
	FString currentStr = FString::Printf(TEXT("%s-%s-%s"), *md->version, *md->configs.serviceAgreementUrl, *md->configs.serviceTermsUrl);
	if (preStr == currentStr)
	{
		return false;
	}
	return true;
}

void FXUInitConfigModel::UpdatePrivacyState()
{
	auto md = GetLocalModel();
	if (md.IsValid())
	{
		FString currentStr = FString::Printf(TEXT("%s-%s-%s"), *md->version, *md->configs.serviceAgreementUrl, *md->configs.serviceTermsUrl);
		TUDataStorage<FXUStorage>::SaveString(FXUStorage::PrivacyKey, currentStr);
	}
}

void FXUInitConfigModel::GetPrivacyTxt(const FString& txtUrl, TFunction<void(FString txt)> callback)
{
	FString *txt = nullptr;
	if (CurrentModel.IsValid())
	{
		txt = CurrentModel->Cache.Find(txtUrl);
	}
	if (txt == nullptr)
	{
		XUNet::RequestPrivacyTxt(txtUrl,
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

void FXUInitConfigModel::SavePrivacyTxt()
{
	GetPrivacyTxt(configs.serviceTermsTxt, nullptr);
	GetPrivacyTxt(configs.serviceAgreementTxt, nullptr);
}
