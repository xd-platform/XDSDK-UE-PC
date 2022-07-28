#pragma once
#include "AAUType.h"

class ANTIADDICTION_API AntiAddictionUE {
public:

	DECLARE_MULTICAST_DELEGATE(SimpleDelegate)

	// 未成年可玩时间耗尽，强制退出游戏，可以做一些保存用户数据的行为
	static SimpleDelegate OnExit;

	// 未成年可玩时间耗尽，可以选择切换账号（配置支持），游戏可以触发退出账号的逻辑
	static SimpleDelegate OnSwitchAccount;

	// 防沉迷初始化
	static void Init(const AAUType::Config& Config);

	// 启动防沉迷
	static void StartUp(const FString& UserID, TFunction<void(AAUType::StartUpResult Result)> CallBack);

	// 进入游戏
	static void EnterGame();

	// 离开游戏
	static void LeaveGame();

	// 防沉迷退出
	static void Logout();

	// 并不是返回用户准确年龄，只是返回一个年龄区间，比如大于18岁，只会返回18；
	static int GetCurrentUserAgeLimite();

	/// 获取用户剩余时长
	static int GetCurrentUserRemainTime();
	

	/// 查询能否支付
	/// - Parameter amount: 支付金额，单位分
	/// - Parameter status: true:可以付费 false:限制消费
	/// - Parameter title: 限制消费时提示标题
	/// - Parameter description: 限制消费提示国家法规内容
	static void CheckPayLimit(int Amount,
		TFunction<void(bool Status, const FString& Title, const FString& Description)> CallBack,
		TFunction<void(const FString& Msg)> FailureHandler);

	/// 上报消费结果 
	/// - Parameter amount: 支付金额，单位分
	static void SubmitPayResult(int Amount,
		TFunction<void(bool Success)> CallBack,
		TFunction<void(const FString& Msg)> FailureHandler);



	static void Test();
	
};

