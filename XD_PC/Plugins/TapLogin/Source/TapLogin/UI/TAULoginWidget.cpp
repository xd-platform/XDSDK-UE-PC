// Fill out your copyright notice in the Description page of Project Settings.
#include "TAULoginWidget.h"

#include "TAULoginLanguage.h"
#include "TAULoginNet.h"
#include "TDSHelper.h"


UTAULoginWidget::UTAULoginWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UTAULoginWidget::ShowLoginUI(TArray<FString> Permissions, TFunction<void(TapAuthResult result)> Completed)
{
	if (UClass* MyWidgetClass = LoadClass<UTAULoginWidget>(nullptr, TEXT("WidgetBlueprint'/TapLogin/BPTapLoginUI.BPTapLoginUI_C'")))
	{
		if (GWorld && GWorld->GetWorld())
		{
			auto widget = CreateWidget<UTAULoginWidget>(GWorld->GetWorld(), MyWidgetClass);
			widget->Permissions = Permissions;
			widget->Completed = Completed;
			widget->AddToViewport();
		}
	}
}

void UTAULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CloseButton->OnClicked.AddUniqueDynamic(this, &UTAULoginWidget::OnCloseBtnClick);
	RefreshButton->OnClicked.AddUniqueDynamic(this, &UTAULoginWidget::OnRefreshBtnClick);
	JumpWebButton->OnClicked.AddUniqueDynamic(this, &UTAULoginWidget::OnJumpWebBtnClick);

	TitleUseLabel->SetText(FText::FromString(TAULoginLanguage::GetCurrentLang()->TitleUse()));
	TitleLoginLabel->SetText(FText::FromString(TAULoginLanguage::GetCurrentLang()->TitleLogin()));
	QrTitleLoginLabel->SetText(FText::FromString(TAULoginLanguage::GetCurrentLang()->QrTitleLogin()));
	QrRefreshLabel->SetText(FText::FromString(TAULoginLanguage::GetCurrentLang()->QrRefresh()));
	QrNoticeUseLabel->SetText(FText::FromString(TAULoginLanguage::GetCurrentLang()->QrNoticeUse()));
	QrNoticeClientLabel->SetText(FText::FromString(TAULoginLanguage::GetCurrentLang()->QrNoticeClient()));
	QrNoticeScanToLoginLabel->SetText(FText::FromString(TAULoginLanguage::GetCurrentLang()->QrNoticeScanToLogin()));
	WebLoginLabel->SetText(FText::FromString(TAULoginLanguage::GetCurrentLang()->WebLogin()));
	WebNoticeLabel->SetText(FText::FromString(TAULoginLanguage::GetCurrentLang()->WebNotice()));
	WebButtonJumpToWebLabel->SetText(FText::FromString(TAULoginLanguage::GetCurrentLang()->WebButtonJumpToWeb()));
	
	RefreshButton->SetVisibility(ESlateVisibility::Hidden);
	QRCoverView->SetVisibility(ESlateVisibility::Hidden);

	auto texture = TDSHelper::GenerateQrCode(QRImage, "https://www.baidu.com");
	QRImage->SetBrushFromTexture(texture);

	// auto langModel = LanguageManager::GetCurrentModel();
	// TitleLabel->SetText(FText::FromString(langModel->tds_terms_agreement));
	// ComfirmButtonLabel->SetText(FText::FromString(langModel->tds_confirm_agreement));
	// AgreeCheckLabel1->SetText(FText::FromString(langModel->tds_service_terms_agreement));
	// AgreeCheckLabel2->SetText(FText::FromString(langModel->tds_service_terms_agreement));

	GetQrCode();
}

void UTAULoginWidget::OnCloseBtnClick()
{
	RemoveFromParent();
	if (Completed)
	{
		Completed(TapAuthResult::CancelInit());
	}
}

void UTAULoginWidget::OnRefreshBtnClick()
{
	TDSHelper::Debug("OnRefreshBtnClick");
}

void UTAULoginWidget::OnJumpWebBtnClick()
{
	TDSHelper::Debug("OnJumpWebBtnClick");
}

void UTAULoginWidget::GetQrCode()
{
	TAULoginNet::RequestLoginQrCode(Permissions,
	[](TSharedPtr<FTAUQrCodeModel> model, FTapError error)
	{
		
	});
}



