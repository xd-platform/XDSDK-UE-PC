#pragma once
#include "XDEnumType.h"
#include "LanguageModel.h"

class LanguageManager
{
public:
	
	static void SetLanguageType(XDLangType type);

	static XDLangType GetCurrentType();

	static TSharedPtr<FLanguageModel> GetCurrentModel();

	static FString GetLanguageKey();

	static FString GetCustomerCenterLang();

private:

	static XDLangType anguageType;
	static TSharedPtr<FLanguageModel> currentModel;

	static void UpdateLanguageModel();
};
