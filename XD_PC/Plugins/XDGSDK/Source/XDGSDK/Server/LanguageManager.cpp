#include "LanguageManager.h"
#include "JsonObjectConverter.h"

static FString LanguageJsonPath = FPaths::ProjectPluginsDir() + TEXT("XDGSDK/Content/Language.json");
LangType LanguageManager::anguageType = LangType::ZH_CN;
TSharedPtr<FLanguageModel> LanguageManager::currentModel = nullptr;

void LanguageManager::SetLanguageType(LangType type)
{
	if (type != anguageType)
	{
		anguageType = type;
		UpdateLanguageModel();
	} 
}

LangType LanguageManager::GetCurrentType()
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
	if (anguageType == LangType::ZH_CN){
		return "zh_CN";
	} else if (anguageType == LangType::ZH_TW){
		return "zh_TW";
	} else if (anguageType == LangType::EN){
		return "en_US";
	} else if (anguageType == LangType::TH){
		return "th_TH";
	} else if (anguageType == LangType::ID){
		return "in_ID";
	} else if (anguageType == LangType::KR){
		return "ko_KR";
	} else if (anguageType == LangType::JP){
		return "ja_JP";
	} else if (anguageType == LangType::DE){
		return "de_DE";
	} else if (anguageType == LangType::FR){
		return "fr_FR";
	} else if (anguageType == LangType::PT){
		return "pt_PT";
	} else if (anguageType == LangType::ES){
		return "es_ES";
	} else if (anguageType == LangType::TR){
		return "tr_TR";
	} else if (anguageType == LangType::RU){
		return "ru_RU";
	}
	return "en_US";
}

FString LanguageManager::GetCustomerCenterLang()
{
	if (anguageType == LangType::ZH_CN){
		return "cn";
	} else if (anguageType == LangType::ZH_TW){
		return "tw";
	} else if (anguageType == LangType::EN){
		return "us";
	} else if (anguageType == LangType::TH){
		return "th";
	} else if (anguageType == LangType::ID){
		return "id";
	} else if (anguageType == LangType::KR){
		return "kr";
	} else if (anguageType == LangType::JP){
		return "jp";
	} else if (anguageType == LangType::DE){
		return "de";
	} else if (anguageType == LangType::FR){
		return "fr";
	} else if (anguageType == LangType::PT){
		return "pt";
	} else if (anguageType == LangType::ES){
		return "es";
	} else if (anguageType == LangType::TR){
		return "tr";
	} else if (anguageType == LangType::RU){
		return "ru";
	}
	return "us";
}

void LanguageManager::UpdateLanguageModel()
{
	FString JsonStr;
	if(FFileHelper::LoadFileToString(JsonStr, *LanguageJsonPath))
	{
		TSharedPtr<FJsonObject> JsonObject;
		const TSharedRef<TJsonReader<>>& JsonReader = TJsonReaderFactory<>::Create(JsonStr);
		FJsonSerializer::Deserialize(JsonReader, JsonObject);
		TSharedPtr<FJsonObject> languageObject = JsonObject->GetObjectField(GetLanguageKey());
		if (languageObject == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("language json content error"));
		} else
		{
			currentModel = MakeShareable(new FLanguageModel);
			FJsonObjectConverter::JsonObjectToUStruct(languageObject.ToSharedRef(), currentModel.Get());
			// UE_LOG(LogTemp, Display, TEXT("%s, %s"), *currentModel->tds_account_bind_info, *currentModel->tds_account_safe_info)
		}

	} else
	{
		UE_LOG(LogTemp, Error, TEXT("not found language json"));
	}
}











