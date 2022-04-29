#include "XDGUserCenterItemWidget.h"

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
}

void UXDGUserCenterItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
