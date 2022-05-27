#include "XDGUserCenterItemWidget.h"

#include "InitConfigModel.h"
#include "LanguageManager.h"

UXDGUserCenterItemWidget::UXDGUserCenterItemWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

UXDGUserCenterItemWidget* UXDGUserCenterItemWidget::GenerateItem()
{
	UXDGUserCenterItemWidget* widget = nullptr;
	if (UClass* MyWidgetClass = LoadClass<UXDGUserCenterItemWidget>(nullptr, TEXT("WidgetBlueprint'/XDGSDK/BPXDGUserCenterItem.BPXDGUserCenterItem_C'")))
	{
		if (GWorld && GWorld->GetWorld())
		{
			widget = CreateWidget<UXDGUserCenterItemWidget>(GWorld->GetWorld(), MyWidgetClass);
		}
	}
	return widget;
}

void UXDGUserCenterItemWidget::SetBindModel(const TSharedPtr<FXDGBindModel>& Model)
{
	BindModel = Model;
	auto langModel = LanguageManager::GetCurrentModel();
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

void UXDGUserCenterItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BindButton->OnClicked.AddUniqueDynamic(this, &UXDGUserCenterItemWidget::OnBindBtnClick);
}

void UXDGUserCenterItemWidget::OnBindBtnClick()
{
	if (BindCallBack)
	{
		BindCallBack(this, BindModel);
	}
}

void UXDGUserCenterItemWidget::ProcessShowOrNot() {
	auto md = FInitConfigModel::GetLocalModel();
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

void UXDGUserCenterItemWidget::ShowBindBt(int Show) {
	if (Show == 1) {
		BindButton->SetVisibility(ESlateVisibility::Visible);
	} else {
		BindButton->SetVisibility(ESlateVisibility::Hidden);
	}
}
