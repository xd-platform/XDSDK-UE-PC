#include "XUPayWebWidget.h"

#include "TUDebuger.h"
#include "TUSettings.h"
#include "URLParser.h"
#include "XULanguageManager.h"
#include "XDUE.h"

UXUPayWebWidget::UXUPayWebWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UXUPayWebWidget::Show(const FString& PayUrl, TFunction<void(XUType::PayResult Result)> CallBack) {
	if (UClass* MyWidgetClass = LoadClass<UXUPayWebWidget>(nullptr, TEXT("WidgetBlueprint'/XDGSDK/BPPayWeb.BPPayWeb_C'")))
	{
		if (TUSettings::GetGameInstance().IsValid()) {
			auto Widget = CreateWidget<UXUPayWebWidget>(TUSettings::GetGameInstance().Get(), MyWidgetClass);
			Widget->PayUrl = PayUrl;
			Widget->CallBack = CallBack;
			Widget->AddToViewport(TUSettings::GetUILevel());
		}
	}
}



void UXUPayWebWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PayWebBrowser->LoadURL(PayUrl);
	CloseButton->OnClicked.AddUniqueDynamic(this, &UXUPayWebWidget::OnCloseClick);
	PayWebBrowser->OnUrlChanged.AddUniqueDynamic(this, &UXUPayWebWidget::OnUrlChanged);
}

void UXUPayWebWidget::OnCloseClick() {
	CloseWithResult(XUType::PayCancel);
}

void UXUPayWebWidget::OnUrlChanged(const FText& Text) {
	TUDebuger::DisplayLog(Text.ToString());
	auto Parse = TUCommon::FURL_RFC3986();
	Parse.Parse(Text.ToString());
	FString Result = Parse.GetFragment();
	if (Result == "success") {
		CloseWithResult(XUType::PaySuccess);
	} else if (Result == "fail") {
		CloseWithResult(XUType::PayFail);
	} else if (Result == "cancel") {
		CloseWithResult(XUType::PayCancel);
	}
}

void UXUPayWebWidget::CloseWithResult(XUType::PayResult Result) {
	if (CallBack) {
		CallBack(Result);
	}
	RemoveFromParent();
}

