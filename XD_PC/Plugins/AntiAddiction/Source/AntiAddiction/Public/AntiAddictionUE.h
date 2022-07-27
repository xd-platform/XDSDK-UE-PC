#pragma once
#include "AAUDelegate.h"
#include "AAUType.h"

class ANTIADDICTION_API AntiAddictionUE {
public:

	static void Init(const AAUType::Config& Config);

	static void StartUp(const FString& UserID, TFunction<void(AAUType::StartUpResult Result)> CallBack);

	static void RegisterDelegate(AAUDelegate* Delegate);

	static void EnterGame();

	static void LeaveGame();

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

