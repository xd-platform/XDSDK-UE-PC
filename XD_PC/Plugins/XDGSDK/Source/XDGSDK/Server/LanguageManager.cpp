#include "LanguageManager.h"
#include "TUJsonHelper.h"
#include "JsonObjectConverter.h"
#include "TUDebuger.h"
#include "XDGSDK.h"

static FString LanguageJsonPath = FPaths::ProjectPluginsDir() / TEXT("XDGSDK/Content") / TEXT("XDGAssets/Language.json");

XDLangType LanguageManager::anguageType = XDLangType::XD_ZH_CN;
TSharedPtr<FLanguageModel> LanguageManager::currentModel = nullptr;

void LanguageManager::SetLanguageType(XDLangType type)
{
	if (type != anguageType)
	{
		anguageType = type;
		UpdateLanguageModel();
	} 
}

XDLangType LanguageManager::GetCurrentType()
{
	return anguageType;
}

TSharedPtr<FLanguageModel> LanguageManager::GetCurrentModel()
{
	if (currentModel == nullptr){
		UpdateLanguageModel();
	}
	return currentModel;
}



FString LanguageManager::GetLanguageKey()
{
	if (anguageType == XDLangType::XD_ZH_CN){
		return "zh_CN";
	} else if (anguageType == XDLangType::XD_ZH_TW){
		return "zh_TW";
	} else if (anguageType == XDLangType::XD_EN){
		return "en_US";
	} else if (anguageType == XDLangType::XD_TH){
		return "th_TH";
	} else if (anguageType == XDLangType::XD_ID){
		return "in_ID";
	} else if (anguageType == XDLangType::XD_KR){
		return "ko_KR";
	} else if (anguageType == XDLangType::XD_JP){
		return "ja_JP";
	} else if (anguageType == XDLangType::XD_DE){
		return "de_DE";
	} else if (anguageType == XDLangType::XD_FR){
		return "fr_FR";
	} else if (anguageType == XDLangType::XD_PT){
		return "pt_PT";
	} else if (anguageType == XDLangType::XD_ES){
		return "es_ES";
	} else if (anguageType == XDLangType::XD_TR){
		return "tr_TR";
	} else if (anguageType == XDLangType::XD_RU){
		return "ru_RU";
	}
	return "en_US";
}

FString LanguageManager::GetCustomerCenterLang()
{
	if (anguageType == XDLangType::XD_ZH_CN){
		return "cn";
	} else if (anguageType == XDLangType::XD_ZH_TW){
		return "tw";
	} else if (anguageType == XDLangType::XD_EN){
		return "us";
	} else if (anguageType == XDLangType::XD_TH){
		return "th";
	} else if (anguageType == XDLangType::XD_ID){
		return "id";
	} else if (anguageType == XDLangType::XD_KR){
		return "kr";
	} else if (anguageType == XDLangType::XD_JP){
		return "jp";
	} else if (anguageType == XDLangType::XD_DE){
		return "de";
	} else if (anguageType == XDLangType::XD_FR){
		return "fr";
	} else if (anguageType == XDLangType::XD_PT){
		return "pt";
	} else if (anguageType == XDLangType::XD_ES){
		return "es";
	} else if (anguageType == XDLangType::XD_TR){
		return "tr";
	} else if (anguageType == XDLangType::XD_RU){
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
			currentModel = MakeShareable(new FLanguageModel);
			if (!FJsonObjectConverter::JsonObjectToUStruct(languageObject->ToSharedRef(), currentModel.Get()))
			{
				XDG_LOG(Error, TEXT("%s language json content error"), *GetLanguageKey());
			}
			
			// UE_LOG(LogTemp, Display, TEXT("%s, %s"), *currentModel->tds_account_bind_info, *currentModel->tds_account_safe_info)
		} else
		{
			XDG_LOG(Error, TEXT("language json content error"));
		}

	} else
	{
		XDG_LOG(Error, TEXT("not found language json"));
	}
}











