#pragma once

namespace AAUType {

	enum StartUpResult {
		StartUpSuccess,		// 可以玩游戏
		TimeLimit,			// 未成年达到时限，强制退出游戏，会触发AAUDelegate代理
		RealNameVerifying,	// 实名认证正在审核中，不可进入游戏
		StartUpCancel,		// 用户取消了实名认证，不可进入游戏
	};

	enum AgeLimit {
		Child = 0,                   
		Teen = 8,                   
		Young = 16,                        
		Adult = 18,                      
	};

	class Config {
	public:
		// 注册获得的Client ID
		FString GameID;

		// 是否使用TapTap的快速实名认证(暂不支持)
		bool UseTapLogin = false;

		// 是否支持切换账号，如果支持，那么请实现AAUDelegate，在代理中实现退出用户账号的逻辑，否则有合规风险
		bool ShowSwitchAccount = false;
	};

}
