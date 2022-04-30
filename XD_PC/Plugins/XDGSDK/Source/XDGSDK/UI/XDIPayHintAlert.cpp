#include "XDIPayHintAlert.h"

#include "LanguageManager.h"

UXDIPayHintAlert::UXDIPayHintAlert(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UXDIPayHintAlert::Show(bool isIOS, bool isAndroid)
{
	if (UClass* MyWidgetClass = LoadClass<UXDIPayHintAlert>(nullptr, TEXT("WidgetBlueprint'/XDGSDK/BPXDIPayHintAlert.BPXDIPayHintAlert_C'")))
	{
		if (GWorld && GWorld->GetWorld())
		{
			auto Widget = CreateWidget<UXDIPayHintAlert>(GWorld->GetWorld(), MyWidgetClass);
			Widget->isIOS = isIOS;
			Widget->isAndroid = isAndroid;
			Widget->AddToViewport();
		}
	}
}

void UXDIPayHintAlert::NativeConstruct()
{
	Super::NativeConstruct();
	auto langModel = LanguageManager::GetCurrentModel();
	TitleLabel->SetText(FText::FromString(langModel->tds_refund_login_restrict_title));
	DetailLabel->SetText(FText::FromString(langModel->tds_refund_login_restrict_sub_title));
	TailLabel->SetText(FText::FromString(langModel->tds_refund_custom_service_tip));
	ButtonLabel->SetText(FText::FromString(langModel->tds_refund_contact_custom_service));

	if (isIOS && isAndroid)
	{
		ContentLabel->SetText(FText::FromString(langModel->tds_refund_all_pay_tip));
	} else if (isIOS)
	{
		ContentLabel->SetText(FText::FromString(langModel->tds_refund_ios_pay_tip));
	} else
	{
		ContentLabel->SetText(FText::FromString(langModel->tds_refund_android_pay_tip));
	}

	
	ContactCustomerServiceButton->OnClicked.AddUniqueDynamic(this, &UXDIPayHintAlert::OnContactCustomerServiceBtnClick);
}

void UXDIPayHintAlert::OnContactCustomerServiceBtnClick()
{
	
}
