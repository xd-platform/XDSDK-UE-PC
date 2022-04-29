#include "XDGUserCenterWidget.h"

#include "TDUDebuger.h"
#include "TDUHUD.h"
#include "XDGNet.h"
#include "XDGUser.h"
#include "XDGUserCenterItemWidget.h"
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
	ErrorButtonLabel->SetText(FText::FromString(langModel->tds_network_error_retry));
	InfoTitleLabel->SetText(FText::FromString(langModel->tds_account_bind_info));
	
	CloseButton->OnClicked.AddUniqueDynamic(this, &UXDGUserCenterWidget::OnCloseBtnClick);
	CopyButton->OnClicked.AddUniqueDynamic(this, &UXDGUserCenterWidget::OnCopyBtnClick);
	ErrorButton->OnClicked.AddUniqueDynamic(this, &UXDGUserCenterWidget::OnErrorBtnClick);
	DeleteButton->OnClicked.AddUniqueDynamic(this, &UXDGUserCenterWidget::OnDeleteBtnClick);

	ShouldShowErrorButton(false);

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

void UXDGUserCenterWidget::OnErrorBtnClick()
{
	RequestList();
}

void UXDGUserCenterWidget::OnDeleteBtnClick()
{
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
	UTDUHUD::ShowWait();
	XDGNet::RequestBindList([=](TSharedPtr<FXDGBindResponseModel> Model, FXDGError Error)
	{
		UTDUHUD::Dismiss();
		if (Model.IsValid())
		{
			ShouldShowErrorButton(false);
			BindModels.Reset();
			auto SupportList = GetSupportTypes();
			for (auto st : SupportList)
			{
				TSharedPtr<FXDGBindModel> md = MakeShareable(new FXDGBindModel);
				md->loginType = st.typeValue;
				md->loginName = st.typeName;
				md->status = (int) FXDGBindType::UnBind;
				for (auto netMd : Model->data)
				{
					if (st.typeValue == netMd.loginType && netMd.status == (int) FXDGBindType::Bind){
						md->status = netMd.status; //1未绑定
						md->bindDate = netMd.bindDate;
						break;
					}
				}
				BindModels.Add(md);
			}
			ResetListBox();
		} else
		{
			ShouldShowErrorButton(true);
			TDUDebuger::WarningLog(Error.msg);
		}
	});
}

void UXDGUserCenterWidget::ResetListBox()
{
	ListBox->ClearChildren();
	for (auto FxdgBindModel : BindModels)
	{
		UXDGUserCenterItemWidget * Item = UXDGUserCenterItemWidget::GenerateItem();
		Item->SetBindModel(FxdgBindModel);
		ListBox->AddChild(Item);
	}
	
}

void UXDGUserCenterWidget::ShouldShowErrorButton(bool Should)
{
	if (Should)
	{
		ListBox->SetVisibility(ESlateVisibility::Collapsed);
		EmptyBox1->SetVisibility(ESlateVisibility::Collapsed);
		EmptyBox2->SetVisibility(ESlateVisibility::Collapsed);
		DeleteButton->SetVisibility(ESlateVisibility::Collapsed);
		ErrorButton->SetVisibility(ESlateVisibility::Visible);
	} else
	{
		ListBox->SetVisibility(ESlateVisibility::Visible);
		EmptyBox1->SetVisibility(ESlateVisibility::Collapsed);
		EmptyBox2->SetVisibility(ESlateVisibility::Visible);
		DeleteButton->SetVisibility(ESlateVisibility::Collapsed);
		ErrorButton->SetVisibility(ESlateVisibility::Collapsed);
	}
}

TArray<FXDGLoginTypeModel> UXDGUserCenterWidget::GetSdkTypes()
{
	TArray<FXDGLoginTypeModel> list;
	list.Add(FXDGLoginTypeModel(LoginType::TapTap));
	return list;
}

TArray<FXDGLoginTypeModel> UXDGUserCenterWidget::GetSupportTypes()
{
	TArray<FXDGLoginTypeModel> list;
	TArray<FXDGLoginTypeModel> SDKList = GetSdkTypes();
	auto md = FInitConfigModel::GetLocalModel();
	for (auto Model : SDKList)
	{
		for (auto BindEntry : md->configs.bindEntries)
		{
			if (Model.typeName.ToLower() == BindEntry.ToLower())
			{
				list.Add(Model);
				break;
			}
		}
	}
	return list;
}
