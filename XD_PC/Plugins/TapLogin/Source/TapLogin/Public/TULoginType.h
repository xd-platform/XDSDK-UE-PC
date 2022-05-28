#pragma once
namespace TULoginType {

	// 这个影响服务的host以及TapTap授权客户端的种类（是国内还是海外）
	enum RegionType {
		CN,
		IO
	};

	enum LanguageType {
		AUTO,   // 海外默认英语，国内默认简体中文
		ZH,		// 简体中文
		EN,		// 英文，海外默认语言
		ID,		// 印尼文
		JA,		// 日语
		KO,		// 韩语
		TH,		// 泰文
		ZHTW,	// 繁体中文
	};


	class Config {
	public:
		FString ClientID;
		RegionType RegionType = CN;
		bool RoundCorner = true;
		LanguageType LanguageType = AUTO;
	};

	class PermissionScope {
	public:
		static FString Profile;
		static FString Friend;
	};
	
};
