#pragma once

class TAPCOMMON_API TULanguage {
public:
	enum Type {
		Unknow = -1,	// 未知
		ZH_CN = 0,		// 中文
		ZH_TW = 1,		// 繁体中文 
		EN = 2,			// 英文
		TH = 3,			// 泰文
		ID = 4,			// 印尼文
		KR = 5,			// 韩语
		JP = 6,			// 日语
		DE = 7,			// 德语
		FR = 8,			// 法语
		PT = 9,			// 葡萄牙语
		ES = 10,		// 西班牙语
		TR = 11,		// 土耳其语
		RU = 12,		// 俄罗斯语
	};

	static Type GetNativeType();
	static Type GetCurrentType();
	static bool SetCurrentType(Type Type);

	DECLARE_EVENT_OneParam(TdsLanguage, LanguageChangedEvent, Type);
	LanguageChangedEvent& OnLanguageChanged() { return LanguageChangedEvent; }


private:
	static TULanguage& Get();
	static TULanguage *SingleInstance;

	TMap<Type, FString> LanguageMap;
	void ParseLanguages();
	LanguageChangedEvent LanguageChangedEvent;
};
