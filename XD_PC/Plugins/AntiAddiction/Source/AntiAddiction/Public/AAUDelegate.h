#pragma once

class AAUDelegate {
public:
	// 未成年可玩时间耗尽，强制退出游戏，可以做一些保存用户数据的行为
	virtual void OnExit() {};

	// 未成年可玩时间耗尽，可以选择切换账号（配置支持），游戏可以触发退出账号的逻辑
	virtual void OnSwitchAccount() {};
};
