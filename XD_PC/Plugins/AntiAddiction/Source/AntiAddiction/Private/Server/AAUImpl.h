#pragma once
#include "AAUServerDelegate.h"
#include "AAUType.h"
#include "TUError.h"
#include "Model/AAURealNameResultModel.h"
#include "UI/AAUManualRealNameWidget.h"

class AAUServer;
class AAUImpl: AAUServerDelegate {
public:
	static TSharedPtr<AAUImpl>& Get();

	AAUType::Config Config;

	static FString LocalTokenString;

	void Init(const AAUType::Config& _Config);
	
	void StartUp(const FString& UserID, TFunction<void(AAUType::StartUpResult Result)> CallBack);
	
	// MARK: - 实名认证部分
	void ManualVerify(const FString& UserID, const FString& Name, const FString& CardID,
		TFunction<void(TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error)> CallBack);

	void CheckRealNameState(const FString& UserID, TFunction<void(TSharedPtr<FAAURealNameResultModel> ModelPtr, const FTUError& Error)> CallBack);

	bool IsNeedStandAlone(TFunction<void(bool IsNeed)> CallBack = nullptr);

	void EnterGame();

	void LeaveGame();

	void Logout();

	// 并不是返回用户准确年龄，只是返回一个年龄区间，比如大于18岁，只会返回18；
	int GetCurrentUserAgeLimite();

	/// 获取用户剩余时长
	int GetCurrentUserRemainTime();
	

	/// 查询能否支付
	/// - Parameter amount: 支付金额，单位分
	/// - Parameter status: true:可以付费 false:限制消费
	/// - Parameter title: 限制消费时提示标题
	/// - Parameter description: 限制消费提示国家法规内容

	void CheckPayLimit(int Amount,
		TFunction<void(bool Status, const FString& Title, const FString& Description)> CallBack,
		TFunction<void(const FString& Msg)> FailureHandler);

	/// 上报消费结果 
	/// - Parameter amount: 支付金额，单位分
	void SubmitPayResult(int Amount,
		TFunction<void(bool Success)> CallBack,
		TFunction<void(const FString& Msg)> FailureHandler);

private:
	static TSharedPtr<AAUImpl> Instance;

	FString CurrentUserID;

	TFunction<void(AAUType::StartUpResult Result)> CurrentStartUpCallBack;

	void PerformStartUpCallBack(AAUType::StartUpResult Result);

	TSharedPtr<bool> StandAloneEnable;
	
	TSharedPtr<AAUServer> Server;

	virtual void KickOut(AAUTimeBoundary Boundary, const FString& Title, const FString& Content) override;
	
	void Login(const FString& AccessToken, int AgeLimit, bool IsFirst = false);

	void ShowRealNameUI(UAAUManualRealNameWidget *Widget, AAURealNameWordType Type);

	void TryAgainStartUp(const FString& ErrMsg);

	void ShowHealthTipUI(const FString& Title, const FString& Content, int RemainTime, AAUHealthTipType Type);
};
