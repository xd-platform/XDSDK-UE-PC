#pragma once
#include "XULanguageModel.h"
#include "XUType.h"

class LanguageManager
{
public:
	
	static void SetLanguageType(XUType::LangType type);

	static XUType::LangType GetCurrentType();

	static TSharedPtr<FXULanguageModel> GetCurrentModel();

	static FString GetLanguageKey();

	static FString GetCustomerCenterLang();

private:

	static XUType::LangType anguageType;
	static TSharedPtr<FXULanguageModel> currentModel;

	static void UpdateLanguageModel();
};
