#include "TULanguage.h"
#include "Internationalization/Culture.h"

TULanguage* TULanguage::SingleInstance = nullptr;

TULanguage& TULanguage::Get() {
	if (SingleInstance == nullptr) {
		SingleInstance = new TULanguage();
		SingleInstance->ParseLanguages();
		FInternationalization::Get().OnCultureChanged().AddLambda([]() {
			SingleInstance->LanguageChangedEvent.Broadcast(SingleInstance->GetCurrentType());
		});
	}
	return *SingleInstance;
}

void TULanguage::ParseLanguages() {
	TArray<FString> Names = FTextLocalizationManager::Get().GetLocalizedCultureNames(ELocalizationLoadFlags::Game);
	for (auto Name : Names) {
		if (Name.StartsWith("zh")) {
			if (Name == "zh-TW") {
				LanguageMap.Add(ZH_TW, Name);
			} else {
				LanguageMap.Add(ZH_CN, Name);
			}
		} else if (Name.StartsWith("en")) {
			LanguageMap.Add(EN, Name);
		} else if (Name.StartsWith("de")) {
			LanguageMap.Add(DE, Name);
		} else if (Name.StartsWith("es")) {
			LanguageMap.Add(ES, Name);
		} else if (Name.StartsWith("fr")) {
			LanguageMap.Add(FR, Name);
		} else if (Name.StartsWith("id")) {
			LanguageMap.Add(ID, Name);
		} else if (Name.StartsWith("ja")) {
			LanguageMap.Add(JP, Name);
		} else if (Name.StartsWith("ko")) {
			LanguageMap.Add(KR, Name);
		} else if (Name.StartsWith("pt")) {
			LanguageMap.Add(PT, Name);
		} else if (Name.StartsWith("ru")) {
			LanguageMap.Add(RU, Name);
		} else if (Name.StartsWith("th")) {
			LanguageMap.Add(TH, Name);
		} else if (Name.StartsWith("tr")) {
			LanguageMap.Add(TR, Name);
		} 
	}
}

TULanguage::Type TULanguage::GetNativeType() {
	FString Name = FTextLocalizationManager::Get().GetNativeCultureName(ELocalizedTextSourceCategory::Game);
	const Type * FindKey = Get().LanguageMap.FindKey(Name);
	if (FindKey == nullptr) {
		return Unknow;
	}
	return *FindKey;
}

TULanguage::Type TULanguage::GetCurrentType() {
	FString Name = FInternationalization::Get().GetCurrentCulture().Get().GetName();
	const Type * FindKey = Get().LanguageMap.FindKey(Name);
	if (FindKey == nullptr) {
		return Unknow;
	}
	return *FindKey;
}

bool TULanguage::SetCurrentType(Type Type) {
	FString* Name = Get().LanguageMap.Find(Type);
	if (Name == nullptr) {
		return false;
	}
	return FInternationalization::Get().SetCurrentCulture(*Name);
}



