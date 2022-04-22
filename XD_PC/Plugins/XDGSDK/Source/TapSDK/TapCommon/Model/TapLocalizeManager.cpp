#include "TapLocalizeManager.h"


TapLocalizeManager* TapLocalizeManager::Singleton = NULL;

TapLocalizeManager& TapLocalizeManager::Get()
{
	if (Singleton == NULL)
	{
		check(IsInGameThread());
		Singleton = new TapLocalizeManager();
	}
	return *Singleton;
}


void TapLocalizeManager::SetCurrentRegion(bool isCn)
{
	Get().regionIsCn = isCn;
}

void TapLocalizeManager::SetCurrentLanguage(TapLanguage Language)
{
	Get().Language = Language;
}

TapLanguage TapLocalizeManager::GetCurrentLanguage()
{
	if (Get().Language != TapLanguage::AUTO)
	{
		return Get().Language;
	}
	Get().Language = GetSystemLanguage();
	if (Get().Language == TapLanguage::AUTO)
	{
		Get().Language = Get().regionIsCn ? TapLanguage::ZH_HANS : TapLanguage::EN;
	}
	return Get().Language;
}

TapLanguage TapLocalizeManager::GetSystemLanguage()
{
	// 暂未实现，需要比对字符串
	// FInternationalization::GetCulture();
	return TapLanguage::AUTO;
}