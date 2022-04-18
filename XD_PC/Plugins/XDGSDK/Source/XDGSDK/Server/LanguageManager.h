#pragma once
#include "LangType.h"
#include "LanguageModel.h"

class LanguageManager
{
public:
	
	static void SetLanguageType(LangType type);

	static LangType GetCurrentType();

	static TSharedPtr<FLanguageModel> GetCurrentModel();

	static FString GetLanguageKey();

	static FString GetCustomerCenterLang();

private:

	static LangType anguageType;
	static TSharedPtr<FLanguageModel> currentModel;

	static void UpdateLanguageModel();
};
