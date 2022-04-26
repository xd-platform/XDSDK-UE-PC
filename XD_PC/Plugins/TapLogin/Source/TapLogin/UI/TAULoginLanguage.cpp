#include "TAULoginLanguage.h"

TSharedPtr<TAULoginLanguage> TAULoginLanguage::Instance = nullptr;

TSharedPtr<TAULoginLanguage>& TAULoginLanguage::Get()
{
	if (!Instance.IsValid())
	{
		Instance = MakeShareable(new TAULoginLanguage);
	}
	return Instance;
}

TAULoginLanguage::TAULoginLanguage()
{
	cn = MakeShareable(new LoginLangCN);
	io = MakeShareable(new LoginLangIO);
}

TSharedPtr<ILoginLang> TAULoginLanguage::GetCurrentLang()
{
	// zh 和 en 2
	FString lan;
	FInternationalization::Get().GetCulture(lan);
	if (lan == "zh")
	{
		return Get()->cn;
	} else
	{
		return Get()->io;
	}
}


