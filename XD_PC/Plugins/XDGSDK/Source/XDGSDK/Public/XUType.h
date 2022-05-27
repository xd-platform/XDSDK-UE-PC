#pragma once


namespace XUType {
	enum RegionType {
		CN,		// 国内
		IO		// 海外
	};

	enum LangType {
		ZH_CN = 0,	// 中文
		ZH_TW = 1,	// 繁体中文
		EN = 2,		// 英文
		TH = 3,		// 泰文
		ID = 4,		// 印尼文
		KR = 5,		// 韩语
		JP = 6,		// 日语
		DE = 7,		// 德语
		FR = 8,		// 法语
		PT = 9,		// 葡萄牙语
		ES = 10,	// 西班牙语
		TR = 11,	// 土耳其语
		RU = 12,	// 俄罗斯语
	};

	enum LoginType {
		Default	= -1,	// 自动登录
		Guest = 0,		// 游客登录
		TapTap = 5,		// TapTap登录
	};

	enum CheckPayType{
		iOS,				// 只有iOS需要补款
		Android,			// 只有Android需要补款
		iOSAndAndroid,		// iOS Android 都需要补款
		None,				// 没有要补款
	};
	
	class Config {
	public:
		FString ClientId;
		RegionType RegionType = CN;
		LangType LangType = ZH_CN;
		FString GameVersion = "1.0.0";
	};
}
