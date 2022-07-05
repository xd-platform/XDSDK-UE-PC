#pragma once
#include "TUType.h"


namespace XUType {
	enum RegionType {
		CN,		// 国内
		Global		// 海外
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
	

struct FacebookConfig {
	FString AppID;
	FString ClientToken;
};


struct LineConfig {
	FString ChannelID;
};


struct TwitterConfig {
	FString ConsumerKey;
	FString ConsumerSecret;
};


struct GoogleConfig {
	FString ClientID;
	FString ReversedClientID;
	FString ApiKey;
	FString GCMSenderID;
	int PlistVersion;
	FString BundleID;
	FString ProjectID;
	FString StorageBucket;
	bool IsAdsEnabled;
	bool IsAnalyticsEnabled;
	bool IsAppInviteEnabled;
	bool IsGCMEnabled;
	bool IsSigninEnabled;
	FString GoogleAppID;
	FString DatabaseURL;
};


struct AdjustEvent {
	FString EventName;
	FString Token;
};
	
struct AdjustConfig {
	FString AppToken;
	TArray<AdjustEvent> Events;

};

struct AppsflyerConfig {
	FString AppID;
	FString DevKey;
};

struct AgreementConfig {
	FString Url;
	FString Version;
	FString Region;
	bool IsKRPushServiceSwitchEnable;
};

struct BindEntriesConfig {
	bool CanBind;
	FString EntryName;
	bool canUnbind;
};
	
class Config {
public:
	/// 区域选择，可选 CN、Global
	RegionType RegionType = CN;
	/// XDGSDK client id
	FString ClientId;
	/// 是否开启 IDFA
	bool IdfaEnabled;

	/// TapSDK 配置
	TUType::Config TapConfig;
	/// 游戏对外名称 显示在 Facebook 登录和客服页面中
	FString GameName;
	/// TapTap 授权权限
	TArray<FString> TapLoginPermissions;

	LangType LangType = ZH_CN;
	FString GameVersion = "1.0.0";

	/*
	 * 以下内容按需配置
	 */
	/// Facebook 配置信息
	FacebookConfig FacebookInfo;
	/// Facebook  授权权限，如果使用 Facebook 登录，必须配置
	TArray<FString> FacebookLoginPersmissions;
	/// Line 配置信息
	LineConfig LineInfo;
	/// Twitter 配置信息
	TwitterConfig TwitterInfo;
	/// Google 配置信息
	GoogleConfig GoogleInfo;
	/// Adjust 配置信息
	AdjustConfig AdjustInfo;
	/// AppsFlyer 配置信息
	AppsflyerConfig AppsflyerInfo;
	/// 统一登录窗口中的品牌向 logo 素材 URL 数组，需要3个，不配置的话默认https://res.xdcdn.net/TDS/Global/res/xd_logo.png
	TArray<FString> Logos;
	/// 统一登录窗口中的登录入口选项
	TArray<FString> LoginEntries;
	/// XDGSDK app id
	FString AppID;
	/// 用户中心窗口中绑定入口选项
	TArray<BindEntriesConfig> BindEntries;
	/// 发行区域
	FString Region;
	/// 客服链接，默认
	FString ReportUrl;
	/// 注销账户链接，默认
	FString LogoutUrl;
	/// 协议内容
	AgreementConfig Agreement;
};
}
