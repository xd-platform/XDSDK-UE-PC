#include "XUPayWebWidget.h"

#include "TUDebuger.h"
#include "TUSettings.h"
#include "XULanguageManager.h"
#include "XDUE.h"

UXUPayWebWidget::UXUPayWebWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UXUPayWebWidget::Show()
{
	if (UClass* MyWidgetClass = LoadClass<UXUPayWebWidget>(nullptr, TEXT("WidgetBlueprint'/XDGSDK/BPPayWeb.BPPayWeb_C'")))
	{
		if (TUSettings::GetGameInstance().IsValid()) {
			auto Widget = CreateWidget<UXUPayWebWidget>(TUSettings::GetGameInstance().Get(), MyWidgetClass);
			// Widget->CheckType = CheckType;
			Widget->AddToViewport(TUSettings::GetUILevel());
		}
	}
}

void UXUPayWebWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PayWebBrowser->LoadURL("https://sdkpay-test.xd.cn/qr-pay/?productSkuCode=com.xd.sdkdemo1.stone30&payAmount=30.0&orderId=&roleId=323499800362549248&appId=1010&product_id%7Ccom.xd.sdkdemo1.stone30&region=CN&lang=zh_CN&serverId=demo_server_id&productName=com.xd.sdkdemo1.stone30");
	
	CloseButton->OnClicked.AddUniqueDynamic(this, &UXUPayWebWidget::OnCloseClick);
	PayWebBrowser->OnUrlChanged.AddUniqueDynamic(this, &UXUPayWebWidget::OnUrlChanged);
}

void UXUPayWebWidget::OnCloseClick() {
	RemoveFromParent();
}

void UXUPayWebWidget::OnUrlChanged(const FText& Text) {
	TUDebuger::DisplayLog(Text.ToString());
}

