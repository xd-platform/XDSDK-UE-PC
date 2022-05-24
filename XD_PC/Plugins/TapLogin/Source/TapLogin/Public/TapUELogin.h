#pragma once
#include "TUError.h"
#include "TULoginProfileModel.h"
#include "TUAccessToken.h"
#include "TUAuthResult.h"
#include "TULoginFriendResult.h"

class TAPLOGIN_API TapUELogin {
public:
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

	// 只能初始化一次
	static void Init(Config Config);

	// 未登录的话为nullptr
	static TSharedPtr<FTULoginProfileModel> GetProfile();

	// 获取失败，ModelPtr为空
	static void FetchProfile(TFunction<void(TSharedPtr<FTULoginProfileModel> ModelPtr, const FTUError& Error)> CallBack);

	static TSharedPtr<FTUAccessToken> GetAccessToken();

	static void Login(TFunction<void(const TUAuthResult& Result)> CallBack);
	
	static void Login(TArray<FString> Permissions, TFunction<void(const TUAuthResult& Result)> CallBack);

	static void Logout();

	// 仅支持PC端
	static void ChangeLanguage(LanguageType LanguageType);

	// 仅支持移动端
	static void GetTestQualification(TFunction<void(bool IsQualified, const FTUError& Error)> CallBack);
	
	static void QueryMutualList(FString Cursor, int Size, TFunction<void(TSharedPtr<FTULoginFriendResult> ModelPtr, const FTUError& Error)> CallBack);


};
