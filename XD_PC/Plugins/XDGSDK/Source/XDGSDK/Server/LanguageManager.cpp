#include "LanguageManager.h"
#include "TUJsonHelper.h"
#include "JsonObjectConverter.h"
#include "TUDebuger.h"
#include "XDGSDK.h"

static FString LanguageJsonPath = FPaths::ProjectPluginsDir() / TEXT("XDGSDK/Content") / TEXT("XDGAssets/Language.json");

XUType::LangType LanguageManager::anguageType = XUType::ZH_CN;
TSharedPtr<FXULanguageModel> LanguageManager::currentModel = nullptr;

void LanguageManager::SetLanguageType(XUType::LangType type)
{
	if (type != anguageType)
	{
		anguageType = type;
		UpdateLanguageModel();
	} 
}

XUType::LangType LanguageManager::GetCurrentType()
{
	return anguageType;
}

TSharedPtr<FXULanguageModel> LanguageManager::GetCurrentModel()
{
	if (currentModel == nullptr){
		UpdateLanguageModel();
	}
	return currentModel;
}



FString LanguageManager::GetLanguageKey()
{
	if (anguageType == XUType::ZH_CN){
		return "zh_CN";
	} else if (anguageType == XUType::ZH_TW){
		return "zh_TW";
	} else if (anguageType == XUType::EN){
		return "en_US";
	} else if (anguageType == XUType::TH){
		return "th_TH";
	} else if (anguageType == XUType::ID){
		return "in_ID";
	} else if (anguageType == XUType::KR){
		return "ko_KR";
	} else if (anguageType == XUType::JP){
		return "ja_JP";
	} else if (anguageType == XUType::DE){
		return "de_DE";
	} else if (anguageType == XUType::FR){
		return "fr_FR";
	} else if (anguageType == XUType::PT){
		return "pt_PT";
	} else if (anguageType == XUType::ES){
		return "es_ES";
	} else if (anguageType == XUType::TR){
		return "tr_TR";
	} else if (anguageType == XUType::RU){
		return "ru_RU";
	}
	return "en_US";
}

FString LanguageManager::GetCustomerCenterLang()
{
	if (anguageType == XUType::ZH_CN){
		return "cn";
	} else if (anguageType == XUType::ZH_TW){
		return "tw";
	} else if (anguageType == XUType::EN){
		return "us";
	} else if (anguageType == XUType::TH){
		return "th";
	} else if (anguageType == XUType::ID){
		return "id";
	} else if (anguageType == XUType::KR){
		return "kr";
	} else if (anguageType == XUType::JP){
		return "jp";
	} else if (anguageType == XUType::DE){
		return "de";
	} else if (anguageType == XUType::FR){
		return "fr";
	} else if (anguageType == XUType::PT){
		return "pt";
	} else if (anguageType == XUType::ES){
		return "es";
	} else if (anguageType == XUType::TR){
		return "tr";
	} else if (anguageType == XUType::RU){
		return "ru";
	}
	return "us";
}

void LanguageManager::UpdateLanguageModel()
{
	FString JsonStr;
	if(FFileHelper::LoadFileToString(JsonStr, *LanguageJsonPath))
	{
		TSharedPtr<FJsonObject> JsonObject = TUJsonHelper::GetJsonObject(JsonStr);
		const TSharedPtr<FJsonObject>* languageObject = nullptr;
		if (JsonObject.IsValid() && JsonObject->TryGetObjectField(GetLanguageKey(), languageObject))
		{
			currentModel = MakeShareable(new FXULanguageModel);
			if (!FJsonObjectConverter::JsonObjectToUStruct(languageObject->ToSharedRef(), currentModel.Get()))
			{
				TUDebuger::ErrorLog(FString::Printf(TEXT("%s language json content error"), *GetLanguageKey()));
			}
			
			// UE_LOG(LogTemp, Display, TEXT("%s, %s"), *currentModel->tds_account_bind_info, *currentModel->tds_account_safe_info)
		} else
		{
			TUDebuger::ErrorLog("language json content error");
		}

	} else
	{
		TUDebuger::ErrorLog("not found language json");
	}
}











