#include "XUUserCenterItemWidget.h"

#include "TUSettings.h"
#include "XUInitConfigModel.h"
#include "XULanguageManager.h"

UXUUserCenterItemWidget::UXUUserCenterItemWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

UXUUserCenterItemWidget* UXUUserCenterItemWidget::GenerateItem()
{
	UXUUserCenterItemWidget* widget = nullptr;
	if (UClass* MyWidgetClass = LoadClass<UXUUserCenterItemWidget>(nullptr, TEXT("WidgetBlueprint'/XDGSDK/BPXUUserCenterItem.BPXUUserCenterItem_C'")))
	{
		if (TUSettings::GetGameInstance().IsValid()) {
			widget = CreateWidget<UXUUserCenterItemWidget>(TUSettings::GetGameInstance().Get(), MyWidgetClass);
		}
	}
	return widget;
}

void UXUUserCenterItemWidget::SetBindModel(const TSharedPtr<FXUBindModel>& Model)
{
	BindModel = Model;
	auto langModel = XULanguageManager::GetCurrentModel();
	FString Content = langModel->tds_account_format.Replace(TEXT("%s"), *Model->loginName);
	TitleLabel->SetText(FText::FromString(Content));
	
	if (Model->loginType == (int)XUType::TapTap)
	{
		UTexture2D *TapTexture = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/XDGSDK/Images/type_icon_tap.type_icon_tap'"));
		TitleImage->SetBrushFromTexture(TapTexture);
	}
	if (Model->status == FXDGBindType::Bind)
	{
		BindLabel->SetText(FText::FromString(langModel->tds_unbind));
		BindLabel->SetColorAndOpacity(FLinearColor(0.6f, 0.6f, 0.6f, 1));
		ArrowImage->SetBrushFromTexture(LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/XDGSDK/Images/arrow_gray.arrow_gray'")));
	} else
	{
		BindLabel->SetText(FText::FromString(langModel->tds_bind));
		BindLabel->SetColorAndOpacity(FLinearColor::Black);
		ArrowImage->SetBrushFromTexture(LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/XDGSDK/Images/arrow_black.arrow_black'")));
	}
}

void UXUUserCenterItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BindButton->OnClicked.AddUniqueDynamic(this, &UXUUserCenterItemWidget::OnBindBtnClick);
}

void UXUUserCenterItemWidget::OnBindBtnClick()
{
	if (BindCallBack)
	{
		BindCallBack(this, BindModel);
	}
}

void UXUUserCenterItemWidget::ProcessShowOrNot() {
	auto md = FXUInitConfigModel::GetLocalModel();
	if (!BindModel.IsValid() || !md.IsValid()) {
		return;
	}
	for (auto BindEntry : md->configs.bindEntriesConfig) {
		if (BindModel->loginName.ToLower() == BindEntry.entryName.ToLower()) {
			if (BindModel->status == FXDGBindType::Bind) {
				ShowBindBt(BindEntry.canBind);
			} else {
				ShowBindBt(BindEntry.canUnbind);
			}
			break;
		}
	}
	
}

void UXUUserCenterItemWidget::ShowBindBt(int Show) {
	if (Show == 1) {
		BindButton->SetVisibility(ESlateVisibility::Visible);
	} else {
		BindButton->SetVisibility(ESlateVisibility::Hidden);
	}
}
