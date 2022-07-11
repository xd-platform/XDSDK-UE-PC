#include "DemoFirstWidget.h"

#include "TapUEMoment.h"
#include "TUDebuger.h"
#include "TUJsonHelper.h"
#include "TUMomentType.h"
#include "XDUE.h"
#include "XUSettings.h"


UDemoFirstWidget::UDemoFirstWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDemoFirstWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	InitButton->OnClicked.AddUniqueDynamic(this, &UDemoFirstWidget::OnInitButtonClick);
	AnonymousLoginBtn->OnClicked.AddUniqueDynamic(this, &UDemoFirstWidget::OnAnonymousLoginBtnClick);
	TapTapLoginBtn->OnClicked.AddUniqueDynamic(this, &UDemoFirstWidget::OnTapTapLoginBtnClick);
	AutoLoginBtn->OnClicked.AddUniqueDynamic(this, &UDemoFirstWidget::OnAutoLoginBtnClick);
	LogoutBtn->OnClicked.AddUniqueDynamic(this, &UDemoFirstWidget::OnLogoutBtnClick);
	ResetPrivacyBtn->OnClicked.AddUniqueDynamic(this, &UDemoFirstWidget::OnResetPrivacyBtnClick);
	UserCenterBtn->OnClicked.AddUniqueDynamic(this, &UDemoFirstWidget::OnUserCenterBtnClick);
	FillingUIBtn->OnClicked.AddUniqueDynamic(this, &UDemoFirstWidget::OnFillingUIBtnClick);
	CheckFillingBtn->OnClicked.AddUniqueDynamic(this, &UDemoFirstWidget::OnCheckFillingBtnClick);
	CustomerCenterBtn->OnClicked.AddUniqueDynamic(this, &UDemoFirstWidget::OnCustomerCenterBtnClick);
	PushEnableBtn->OnClicked.AddUniqueDynamic(this, &UDemoFirstWidget::OnPushEnableBtnClick);
	MomentBtn->OnClicked.AddUniqueDynamic(this, &UDemoFirstWidget::OnMomentBtnClick);
	WebPayBtn->OnClicked.AddUniqueDynamic(this, &UDemoFirstWidget::OnWebPayBtnClick);

	EnvironmentBox->OnSelectionChanged.AddUniqueDynamic(this, &UDemoFirstWidget::OnEnvironmentBoxSelectionChanged);
	RegionBox->OnSelectionChanged.AddUniqueDynamic(this, &UDemoFirstWidget::OnRegionBoxSelectionChanged);
	LangBox->OnSelectionChanged.AddUniqueDynamic(this, &UDemoFirstWidget::OnLangBoxSelectionChanged);

	WebPayAmountTF->SetText(FText::FromString("30.0"));
	WebPayProductNameTF->SetText(FText::FromString("com.xd.sdkdemo1.stone30"));
	WebPayProductIDTF->SetText(FText::FromString("com.xd.sdkdemo1.stone30"));
	WebPayServerIDTF->SetText(FText::FromString("demo_server_id"));
	WebPayRoleIDTF->SetText(FText::FromString("323499800362549248"));

	XUSettings::UpdateConfigFileName("XDConfig-cn.json");
	XDUE::SetLanguage(XUType::ZH_CN);
	TUDebuger::IsTest = false;

	TUDebuger::ReplaceHosts.Add("https://xdsdk-6.xd.cn", "https://test-xdsdk-6.xd.cn");
	TUDebuger::ReplaceHosts.Add("https://xdsdk-intnl-6.xd.com", "https://test-xdsdk-intnl-6.xd.com");
	TUDebuger::ReplaceHosts.Add("https://ecdn-xdsdk-intnl-6.xd.com", "https://test-xdsdk-intnl-6.xd.com");
	TUDebuger::ReplaceHosts.Add("https://event-tracking-cn.cn-beijing.log.aliyuncs.com/logstores/sdk6-prod/track", "https://event-tracking-cn.cn-beijing.log.aliyuncs.com/logstores/sdk6-test/track");
	TUDebuger::ReplaceHosts.Add("https://event-tracking-global.ap-southeast-1.log.aliyuncs.com/logstores/sdk6-prod/track", "https://event-tracking-global.ap-southeast-1.log.aliyuncs.com/logstores/sdk6-test/track");

	// PayWebBrowser->LoadURL("https://sdkpay-test.xd.cn/qr-pay/?productSkuCode=com.xd.sdkdemo1.stone30&payAmount=30.0&orderId=&roleId=323499800362549248&appId=1010&product_id%7Ccom.xd.sdkdemo1.stone30&region=CN&lang=zh_CN&serverId=demo_server_id&productName=com.xd.sdkdemo1.stone30");
}

void UDemoFirstWidget::OnInitButtonClick() {
	XDUE::InitSDK("1.2.3", [](bool Result, FString Message)
	{
		if (Result)
		{
			TUDebuger::DisplayShow(Message);
		} else
		{
			TUDebuger::WarningShow(Message);
		}
	});
}

void UDemoFirstWidget::OnAnonymousLoginBtnClick() {
	XDUE::LoginByType(XUType::Guest, [](FXUUser User)
	{
		TUDebuger::DisplayShow(TEXT("登录成功：") + TUJsonHelper::GetJsonString(User));
	}, [](FXUError Error)
	{
		TUDebuger::WarningShow(TEXT("登录失败：") + Error.msg);
	});
}

void UDemoFirstWidget::OnTapTapLoginBtnClick() {
	XDUE::LoginByType(XUType::TapTap, [](FXUUser User)
	{
		TUDebuger::DisplayShow(TEXT("登录成功：") + TUJsonHelper::GetJsonString(User));
	}, [](FXUError Error)
	{
		TUDebuger::WarningShow(TEXT("登录失败：") + Error.msg);
	});
}

void UDemoFirstWidget::OnAutoLoginBtnClick() {
	XDUE::LoginByType(XUType::Default, [](FXUUser User)
	{
		TUDebuger::DisplayShow(TEXT("登录成功：") + TUJsonHelper::GetJsonString(User));
	}, [](FXUError Error)
	{
		TUDebuger::WarningShow(TEXT("登录失败：") + Error.msg);
	});
}

void UDemoFirstWidget::OnLogoutBtnClick() {
	XDUE::Logout();
	TUDebuger::DisplayShow(TEXT("退出登录"));
}

void UDemoFirstWidget::OnResetPrivacyBtnClick() {
	XDUE::ResetPrivacy();
	TUDebuger::DisplayShow(TEXT("重置隐私"));
}

void UDemoFirstWidget::OnUserCenterBtnClick() {
	XDUE::OpenUserCenter(
		[](XUType::LoginType Type, TSharedPtr<FXUError> Error) {
			if (Error.IsValid()) {
				TUDebuger::DisplayShow( FString::Printf(TEXT("绑定失败, Error: %s"), *Error->msg));
			}
			else {
				TUDebuger::DisplayShow( FString::Printf(TEXT("绑定成功, Type: %d"), Type));
			}
		},
		[](XUType::LoginType Type, TSharedPtr<FXUError> Error) {
			if (Error.IsValid()) {
				TUDebuger::DisplayShow( FString::Printf(TEXT("解绑失败, Error: %s"), *Error->msg));
			}
			else {
				TUDebuger::DisplayShow( FString::Printf(TEXT("解绑成功, Type: %d"), Type));
			}
		}
	);
}

void UDemoFirstWidget::OnFillingUIBtnClick() {
	XDUE::OpenPayHintAlert();
}

void UDemoFirstWidget::OnCheckFillingBtnClick() {
	XDUE::CheckPay([](XUType::CheckPayType CheckType)
	{
		switch (CheckType)
		{
		case XUType::iOSAndAndroid:
			TUDebuger::DisplayShow(TEXT("iOSAndAndroid"));
			break;
		case XUType::iOS:
			TUDebuger::DisplayShow(TEXT("iOS"));
			break;
		case XUType::Android:
			TUDebuger::DisplayShow(TEXT("Android"));
			break;
		case XUType::None:
			TUDebuger::DisplayShow(TEXT("None"));
			break;
		}
	}, [](FXUError Error)
	{
		TUDebuger::DisplayShow(Error.msg);
	});
}

void UDemoFirstWidget::OnCustomerCenterBtnClick() {
	XDUE::OpenCustomerCenter("serverId", "roleId", "roleName");
}

void UDemoFirstWidget::OnPushEnableBtnClick() {
	if (XDUE::IsPushServiceEnable()) {
		TUDebuger::DisplayShow("Push Service Enable");
	} else {
		TUDebuger::DisplayShow("Push Service Disable");
	}
}

void UDemoFirstWidget::OnMomentBtnClick() {
	TUMomentType::Config Config;
	FString AppId = MomentTF->GetText().ToString();
	Config.RegionType = TUType::Config::Get()->RegionType;
	if (Config.RegionType == TUType::CN) {
		Config.AppID = AppId;
	} else {
		Config.AppID = AppId;
	}
	TapUEMoment::Init(Config);
	TapUEMoment::OpenWebTopic();
}

void UDemoFirstWidget::OnWebPayBtnClick() {
	FString ServerId = WebPayServerIDTF->GetText().ToString();
	FString RoleId = WebPayRoleIDTF->GetText().ToString();
	FString ProductSkuCode = WebPayProductIDTF->GetText().ToString();
	FString ProductName = WebPayProductNameTF->GetText().ToString();
	FString PayAmount = WebPayAmountTF->GetText().ToString();
	XDUE::OpenWebPay(ServerId, RoleId, ProductSkuCode, [](XUType::PayResult Result) {
		switch (Result) {
		case XUType::PaySuccess:
			TUDebuger::DisplayShow("Pay Success");
			break;
		case XUType::PayFail:
			TUDebuger::DisplayShow("Pay Fail");
			break;
		case XUType::PayCancel:
			TUDebuger::DisplayShow("Pay Cancel");
			break;
		}
	}, ProductName, FCString::Atof(*PayAmount), "");
}

void UDemoFirstWidget::OnEnvironmentBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType) {
	TUDebuger::DisplayLog(SelectedItem);
	if (SelectedItem == TEXT("RND环境")) {
		TUDebuger::IsTest = true;
	} else if (SelectedItem == TEXT("正式环境")) {
		TUDebuger::IsTest = false;
	}
}

void UDemoFirstWidget::OnRegionBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType) {
	TUDebuger::DisplayLog(SelectedItem);
	if (SelectedItem == TEXT("国内")) {
		XUSettings::UpdateConfigFileName("XDConfig-cn.json");
	} else if (SelectedItem == TEXT("海外")) {
		XUSettings::UpdateConfigFileName("XDConfig.json");
	}
}

void UDemoFirstWidget::OnLangBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType) {
	TUDebuger::DisplayLog(SelectedItem);
	if (SelectedItem == TEXT("中文")) {
		XDUE::SetLanguage(XUType::ZH_CN);
	} else if (SelectedItem == TEXT("繁体中文")) {
		XDUE::SetLanguage(XUType::ZH_TW);
	} else if (SelectedItem == TEXT("英文")) {
		XDUE::SetLanguage(XUType::EN);
	} else if (SelectedItem == TEXT("泰文")) {
		XDUE::SetLanguage(XUType::TH);
	} else if (SelectedItem == TEXT("印尼文")) {
		XDUE::SetLanguage(XUType::ID);
	} else if (SelectedItem == TEXT("韩语")) {
		XDUE::SetLanguage(XUType::KR);
	} else if (SelectedItem == TEXT("日语")) {
		XDUE::SetLanguage(XUType::JP);
	} else if (SelectedItem == TEXT("德语")) {
		XDUE::SetLanguage(XUType::DE);
	} else if (SelectedItem == TEXT("法语")) {
		XDUE::SetLanguage(XUType::FR);
	} else if (SelectedItem == TEXT("葡萄牙语")) {
		XDUE::SetLanguage(XUType::PT);
	} else if (SelectedItem == TEXT("西班牙语")) {
		XDUE::SetLanguage(XUType::ES);
	} else if (SelectedItem == TEXT("土耳其语")) {
		XDUE::SetLanguage(XUType::TR);
	} else if (SelectedItem == TEXT("俄罗斯语")) {
		XDUE::SetLanguage(XUType::RU);
	} 
}
