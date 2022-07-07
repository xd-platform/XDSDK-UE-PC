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
	ComfirmButtonLabel->SetText(FText::FromString(langModel->tds_confirm_agreement));
	DeclineButtonLabel->SetText(FText::FromString(langModel->tds_decline_agreement));

	if (IsInKrAndPushEnable()) {
		AdditionalCheckLabel->SetText(FText::FromString(langModel->tds_push_agreement));
	} else if (IsInNorthAmerica()) {
		AdditionalCheckLabel->SetText(FText::FromString(langModel->tds_is_adult_agreement));
	} else {
		AdditionalCheckBox->SetVisibility(ESlateVisibility::Collapsed);
	}

	PrivacyWebBrowser->OnUrlChanged.AddUniqueDynamic(this, &UXUPrivacyWidget::OnUrlChanged);
	UpdateComfirmBtnState();
}

void UXUPrivacyWidget::OnCheckStateChanged(bool isChecked)
{
	UpdateComfirmBtnState();
}

void UXUPrivacyWidget::OnComfirmBtnClick()
{
	if (IsInNorthAmerica() && !AdditionalCheckBox->IsChecked()) {
		UTUHUD::ShowToast(XULanguageManager::GetCurrentModel()->tds_service_na_toast);
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

void UXUPrivacyWidget::OnUrlChanged(const FText& Text) {
	TUDebuger::DisplayLog(FString::Printf(TEXT("OnUrlChanged: %s"), *Text.ToString()));
	if (OriginURL == Text.ToString()) {
		return;
	}
	FPlatformProcess::LaunchURL(*Text.ToString(), nullptr, nullptr);
	PrivacyWebBrowser->LoadURL(OriginURL);
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


