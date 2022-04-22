#pragma once
#include "TapLanguage.h"

class TAPCOMMON_API TapLocalizeManager
{
public:
	static void SetCurrentRegion(bool isCn);
	static void SetCurrentLanguage(TapLanguage Language);
	static TapLanguage GetCurrentLanguage();

private:

	TapLanguage Language = TapLanguage::AUTO;
	bool regionIsCn = true;

	static TapLocalizeManager& Get();
	static TapLocalizeManager* Singleton;

	static TapLanguage GetSystemLanguage();
};
