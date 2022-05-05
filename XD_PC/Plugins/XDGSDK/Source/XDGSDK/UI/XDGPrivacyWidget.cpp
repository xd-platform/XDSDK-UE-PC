// Fill out your copyright notice in the Description page of Project Settings.


#include "XDGPrivacyWidget.h"

#include "InitConfigModel.h"
#include "LanguageManager.h"
#include "TDSHelper.h"


UXDGPrivacyWidget::UXDGPrivacyWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UXDGPrivacyWidget::ShowPrivacy(TFunction<void(bool result)> Completed)
{
	if (UClass* MyWidgetClass = LoadClass<UXDGPrivacyWidget>(nullptr, TEXT("WidgetBlueprint'/XDGSDK/BPPrivacyUI.BPPrivacyUI_C'")))
	{
		if (GWorld && GWorld->GetWorld())
		{
			auto widget = CreateWidget<UXDGPrivacyWidget>(GWorld->GetWorld(), MyWidgetClass);
			widget->Completed = Completed;
			widget->AddToViewport();
		}
	}
}

void UXDGPrivacyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ComfirmButton->SetIsEnabled(false);
	
	AgreeCheckBox1->OnCheckStateChanged.AddUniqueDynamic(this, &UXDGPrivacyWidget::OnCheckStateChanged);
	AgreeCheckBox2->OnCheckStateChanged.AddUniqueDynamic(this, &UXDGPrivacyWidget::OnCheckStateChanged);

	FScriptDelegate BtnDel;
	BtnDel.BindUFunction(this, "OnComfirmBtnClick");
	ComfirmButton->OnClicked.Add(BtnDel);

	FInitConfigModel::GetPrivacyTxt(FInitConfigModel::GetLocalModel()->configs.serviceAgreementTxt,
	[=](FString txt)
	{
		if (PrivacyTextView1)
		{
			txt = txt.Replace(TEXT("</b>"), TEXT("</>"));
			PrivacyTextView1->SetText(FText::FromString(txt));
		}
	});
	FInitConfigModel::GetPrivacyTxt(FInitConfigModel::GetLocalModel()->configs.serviceTermsTxt,
	[=](FString txt)
	{
		if (PrivacyTextView2)
		{
			txt = txt.Replace(TEXT("</b>"), TEXT("</>"));
			PrivacyTextView2->SetText(FText::FromString(txt));
		}
		
	});
	auto langModel = LanguageManager::GetCurrentModel();
	TitleLabel->SetText(FText::FromString(langModel->tds_terms_agreement));
	ComfirmButtonLabel->SetText(FText::FromString(langModel->tds_confirm_agreement));
	AgreeCheckLabel1->SetText(FText::FromString(langModel->tds_service_terms_agreement));
	AgreeCheckLabel2->SetText(FText::FromString(langModel->tds_service_terms_agreement));
	
}

void UXDGPrivacyWidget::OnCheckStateChanged(bool isChecked)
{
	if (AgreeCheckBox1->IsChecked() && AgreeCheckBox2->IsChecked())
	{
		ComfirmButton->SetIsEnabled(true);
	} else
	{
		ComfirmButton->SetIsEnabled(false);
	}
}

void UXDGPrivacyWidget::OnComfirmBtnClick()
{
	FInitConfigModel::UpdatePrivacyState();
	if (Completed)
	{
		Completed(true);
	}
	RemoveFromParent();
}
