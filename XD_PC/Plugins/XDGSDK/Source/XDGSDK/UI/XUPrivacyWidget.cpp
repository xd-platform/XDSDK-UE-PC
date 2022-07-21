// Fill out your copyright notice in the Description page of Project Settings.


#include "XUPrivacyWidget.h"

#include "XUServerConfig.h"
#include "XULanguageManager.h"
#include "TUHelper.h"
#include "TUHUD.h"
#include "TUSettings.h"
#include "XUConfigManager.h"
#include "XUPrivacyDisagreeWidget.h"
#include "XUUser.h"


UXUPrivacyWidget::UXUPrivacyWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UXUPrivacyWidget::ShowPrivacy(TFunction<void()> Completed)
{
	if (UClass* MyWidgetClass = LoadClass<UXUPrivacyWidget>(nullptr, TEXT("WidgetBlueprint'/XDGSDK/BPPrivacyUI.BPPrivacyUI_C'")))
	{
		if (TUSettings::GetGameInstance().IsValid()) {
			auto widget = CreateWidget<UXUPrivacyWidget>(TUSettings::GetGameInstance().Get(), MyWidgetClass);
			widget->Completed = Completed;
			widget->AddToViewport(TUSettings::GetUILevel());
		}
	}
}

void UXUPrivacyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	AdditionalCheckBox->OnCheckStateChanged.AddUniqueDynamic(this, &UXUPrivacyWidget::OnCheckStateChanged);

	FScriptDelegate BtnDel;
	BtnDel.BindUFunction(this, "OnComfirmBtnClick");
	ComfirmButton->OnClicked.Add(BtnDel);
	DeclineButton->OnClicked.AddUniqueDynamic(this, &UXUPrivacyWidget::OnDeclineBtnClick);

	OriginURL =  XUConfigManager::GetAgreementUrl();
	PrivacyWebBrowser->LoadURL(OriginURL);
	
	auto langModel = XULanguageManager::GetCurrentModel();
	ComfirmButtonLabel->SetText(FText::FromString(langModel->xd_agreement_agree));
	DeclineButtonLabel->SetText(FText::FromString(langModel->xd_agreement_disagree));

	if (IsInKrAndPushEnable()) {
		AdditionalCheckLabel->SetText(FText::FromString(langModel->tds_push_agreement));
	} else if (IsInNorthAmerica()) {
		AdditionalCheckLabel->SetText(FText::FromString(langModel->tds_is_adult_agreement));
	} else {
		AdditionalCheckBox->SetVisibility(ESlateVisibility::Collapsed);
	}

	PrivacyWebBrowser->OnLoadCompleted.AddUniqueDynamic(this, &UXUPrivacyWidget::OnWebLoadCompleted);
	PrivacyWebBrowser->OnLoadError.AddUniqueDynamic(this, &UXUPrivacyWidget::OnWebLoadError);
	PrivacyWebBrowser->OnLoadStarted.AddUniqueDynamic(this, &UXUPrivacyWidget::OnWebLoadStarted);
	PrivacyWebBrowser->OnBeforeNavigation.BindUObject(this, &UXUPrivacyWidget::OnWebBeforeNavigation);

	UpdateComfirmBtnState();
}

void UXUPrivacyWidget::OnCheckStateChanged(bool isChecked)
{
	UpdateComfirmBtnState();
}

void UXUPrivacyWidget::OnComfirmBtnClick()
{
	if (IsInNorthAmerica() && !AdditionalCheckBox->IsChecked()) {
		UTUHUD::ShowToast(XULanguageManager::GetCurrentModel()->xd_agreement_age_tips);
		return;
	} 

	if (IsInKrAndPushEnable()) {
		XUConfigManager::RecordKRPushSetting(AdditionalCheckBox->IsChecked());
	}
	if (Completed) {
		Completed();
	}
	RemoveFromParent();
}

void UXUPrivacyWidget::OnDeclineBtnClick() {
	UXUPrivacyDisagreeWidget::Show();
}

void UXUPrivacyWidget::OnWebLoadStarted() {
	TUDebuger::DisplayLog("Privacy Web Load Started");
	if (PrivacyWebBrowser->GetUrl() == OriginURL) {
		TUDebuger::DisplayLog("is OriginURL");
	} else {
		TUDebuger::DisplayLog("is not OriginURL");
	}
}

void UXUPrivacyWidget::OnWebLoadCompleted() {
	TUDebuger::DisplayLog("Privacy Web Load Completed");
	if (PrivacyWebBrowser->GetUrl() == OriginURL) {
		TUDebuger::DisplayLog("is OriginURL");
	} else {
		TUDebuger::DisplayLog("is not OriginURL");
	}
}

void UXUPrivacyWidget::OnWebLoadError() {
	TUDebuger::DisplayLog("Privacy Web Load Error");
	if (PrivacyWebBrowser->GetUrl() == OriginURL) {
		TUDebuger::DisplayLog("is OriginURL");
	} else {
		TUDebuger::DisplayLog("is not OriginURL");
	}
}

bool UXUPrivacyWidget::OnWebBeforeNavigation(const FString& Url, const FWebNavigationRequest& Request) {
	TUDebuger::DisplayLog("OnWebBeforeNavigation");
	if (OriginURL == Url) {
		return false;
	}  else {
		FPlatformProcess::LaunchURL(*Url, nullptr, nullptr);
		return true;
	}
}

bool UXUPrivacyWidget::IsInKrAndPushEnable() {
	return XUConfigManager::IsGameInKoreaAndPushServiceEnable();
}

bool UXUPrivacyWidget::IsInNorthAmerica() {
	return XUConfigManager::IsGameInNA();
}

void UXUPrivacyWidget::UpdateComfirmBtnState() {
	if (IsInNorthAmerica() && !AdditionalCheckBox->IsChecked()) {
		ComfirmButtonImage->SetBrushFromTexture(LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/TapLogin/Image/taptap-router-gray.taptap-router-gray'")));
	} else {
		ComfirmButtonImage->SetBrushFromTexture(LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/TapLogin/Image/taptap-router.taptap-router'")));
	}
}


