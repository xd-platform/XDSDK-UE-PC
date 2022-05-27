#include "XDIPayHintAlert.h"

#include "XULanguageManager.h"
#include "XDUE.h"

UXDIPayHintAlert::UXDIPayHintAlert(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UXDIPayHintAlert::Show(XUType::CheckPayType CheckType)
{
	if (CheckType == XUType::None)
	{
		return;
	}
	if (UClass* MyWidgetClass = LoadClass<UXDIPayHintAlert>(nullptr, TEXT("WidgetBlueprint'/XDGSDK/BPXDIPayHintAlert.BPXDIPayHintAlert_C'")))
	{
		if (GWorld && GWorld->GetWorld())
		{
			auto Widget = CreateWidget<UXDIPayHintAlert>(GWorld->GetWorld(), MyWidgetClass);
			Widget->CheckType = CheckType;
			Widget->AddToViewport();
		}
	}
}

void UXDIPayHintAlert::NativeConstruct()
{
	Super::NativeConstruct();
	auto langModel = XULanguageManager::GetCurrentModel();
	TitleLabel->SetText(FText::FromString(langModel->tds_refund_login_restrict_title));
	DetailLabel->SetText(FText::FromString(langModel->tds_refund_login_restrict_sub_title));
	TailLabel->SetText(FText::FromString(langModel->tds_refund_custom_service_tip));
	ButtonLabel->SetText(FText::FromString(langModel->tds_refund_contact_custom_service));

	if (CheckType == XUType::iOSAndAndroid)
	{
		ContentLabel->SetText(FText::FromString(langModel->tds_refund_all_pay_tip));
	} else if (CheckType == XUType::iOS)
	{
		ContentLabel->SetText(FText::FromString(langModel->tds_refund_ios_pay_tip));
	} else if (CheckType == XUType::Android)
	{
		ContentLabel->SetText(FText::FromString(langModel->tds_refund_android_pay_tip));
	} else
	{
		ContentLabel->SetText(FText::FromString(""));
	}

	
	ContactCustomerServiceButton->OnClicked.AddUniqueDynamic(this, &UXDIPayHintAlert::OnContactCustomerServiceBtnClick);
}

void UXDIPayHintAlert::OnContactCustomerServiceBtnClick()
{
	XDUE::OpenCustomerCenter("", "", "");
}
