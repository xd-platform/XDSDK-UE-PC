#include "XDGUserCenterWidget.h"

#include "TDUDebuger.h"
#include "XDGNet.h"
#include "XDGUser.h"
#include "Mac/MacPlatformApplicationMisc.h"

UXDGUserCenterWidget::UXDGUserCenterWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UXDGUserCenterWidget::ShowWidget()
{
	if (UClass* MyWidgetClass = LoadClass<UXDGUserCenterWidget>(nullptr, TEXT("WidgetBlueprint'/XDGSDK/BPXDGUserCenter.BPXDGUserCenter_C'")))
	{
		if (GWorld && GWorld->GetWorld())
		{
			auto widget = CreateWidget<UXDGUserCenterWidget>(GWorld->GetWorld(), MyWidgetClass);
			widget->AddToViewport();
		}
	}
}

void UXDGUserCenterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	userMd = FXDGUser::GetLocalModel();
	langModel = LanguageManager::GetCurrentModel();

	TitleLabel->SetText(FText::FromString(langModel->tds_account_safe_info));
	InfoTitleLabel->SetText(FText::FromString(langModel->tds_account_info));
	FString Content = FString::Printf(TEXT("%s (%s)"), *langModel->tds_current_account_prefix, *GetLoginTypeName());
	CurrentLoginTitleLabel->SetText(FText::FromString(Content));
	Content = "ID: " + userMd->userId;
	IDTitleLabel->SetText(FText::FromString(Content));
	
	// errorView.GetComponentInChildren<Text>().text = langModel.tds_network_error_retry;

	CloseButton->OnClicked.AddUniqueDynamic(this, &UXDGUserCenterWidget::OnCloseBtnClick);

	RequestList();
}

void UXDGUserCenterWidget::OnCloseBtnClick()
{
	RemoveFromParent();
}

void UXDGUserCenterWidget::OnCopyBtnClick()
{
	FPlatformApplicationMisc::ClipboardCopy(*userMd->userId);
	TDUDebuger::DisplayShow(langModel->tds_copy_success);
}

FString UXDGUserCenterWidget::GetLoginTypeName()
{
	FString result = langModel->tds_guest;
	if (userMd->GetLoginType() == LoginType::TapTap)
	{
		result = "TapTap";
	}
	FString unitStr = langModel->tds_account_format;
	return unitStr.Replace(TEXT("%s"), *result);
}

void UXDGUserCenterWidget::RequestList()
{
	XDGNet::RequestBindList([=](TSharedPtr<FXDGBindResponseModel> Model, FXDGError Error)
	{
		TDUDebuger::DisplayLog(JsonHelper::GetJsonString(Model));
	});
}
