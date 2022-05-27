#include "XDGUserCenterWidget.h"

#include "TUDebuger.h"
#include "TUHUD.h"
#include "XUImpl.h"
#include "XUNet.h"
#include "XUUser.h"
#include "XDGUserCenterTipWidget.h"
#include "XDUE.h"
#include "HAL/PlatformApplicationMisc.h"

UXDGUserCenterWidget::UXDGUserCenterWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UXDGUserCenterWidget::ShowWidget(TFunction<void(XUType::LoginType Type, TSharedPtr<FXUError>)> BindCallBack,
	TFunction<void(XUType::LoginType Type, TSharedPtr<FXUError>)> UnbindCallBack) {
	if (UClass* MyWidgetClass = LoadClass<UXDGUserCenterWidget>(nullptr, TEXT("WidgetBlueprint'/XDGSDK/BPXDGUserCenter.BPXDGUserCenter_C'")))
	{
		if (GWorld && GWorld->GetWorld())
		{
			auto widget = CreateWidget<UXDGUserCenterWidget>(GWorld->GetWorld(), MyWidgetClass);
			widget->AddToViewport();
			widget->BindCallBack = BindCallBack;
			widget->UnbindCallBack = UnbindCallBack;
		}
	}
}


void UXDGUserCenterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	userMd = FXUUser::GetLocalModel();
	langModel = XULanguageManager::GetCurrentModel();

	TitleLabel->SetText(FText::FromString(langModel->tds_account_safe_info));
	InfoTitleLabel->SetText(FText::FromString(langModel->tds_account_info));
	FString Content = FString::Printf(TEXT("%s (%s)"), *langModel->tds_current_account_prefix, *GetLoginTypeName());
	CurrentLoginTitleLabel->SetText(FText::FromString(Content));
	Content = "ID: " + userMd->userId;
	IDTitleLabel->SetText(FText::FromString(Content));
	ErrorButtonLabel->SetText(FText::FromString(langModel->tds_network_error_retry));
	BindInfoTitleLabel->SetText(FText::FromString(langModel->tds_account_bind_info));
	DeleteButtonLabel->SetText(FText::FromString(langModel->tds_delete_account));
	
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
	UTUHUD::ShowToast(langModel->tds_copy_success);
}

void UXDGUserCenterWidget::OnErrorBtnClick()
{
	RequestList();
}

void UXDGUserCenterWidget::OnDeleteBtnClick()
{
	if (userMd->loginType != (int)XUType::Guest)
	{
		return;
	}
	UXDGUserCenterTipWidget::Show(UXDGUserCenterTipWidget::DeleteGuest, XUType::Guest, [=]()
	{
		UTUHUD::ShowWait();
		XUNet::Unbind((int)XUType::Guest, [=](TSharedPtr<FXUResponseModel> Model, FXUError Error)
		{
			UTUHUD::Dismiss();
			if (Model.IsValid())
			{
				DeleteAccount(langModel->tds_unbind_guest_return);
			} else
			{
				if (Error.code > 200)
				{
					UTUHUD::ShowToast(Error.msg);
				} else
				{
					UTUHUD::ShowToast(langModel->tds_unbind_guest_return);
				}
				
			}
		});
	}, nullptr);
}

FString UXDGUserCenterWidget::GetLoginTypeName()
{
	FString result = langModel->tds_guest;
	if (userMd->GetLoginType() == XUType::TapTap)
	{
		result = "TapTap";
	}
	FString unitStr = langModel->tds_account_format;
	return unitStr.Replace(TEXT("%s"), *result);
}

void UXDGUserCenterWidget::RequestList()
{
	UTUHUD::ShowWait();
	XUNet::RequestBindList([=](TSharedPtr<FXUBindResponseModel> Model, FXUError Error)
	{
		UTUHUD::Dismiss();
		if (Model.IsValid())
		{
			ShouldShowErrorButton(false);
			BindModels.Reset();
			auto SupportList = GetSupportTypes();
			for (auto st : SupportList)
			{
				TSharedPtr<FXUBindModel> md = MakeShareable(new FXUBindModel);
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
			ResetListBoxAndDeleteButton();
		} else
		{
			ShouldShowErrorButton(true);
			TUDebuger::WarningLog(Error.msg);
		}
	});
}

void UXDGUserCenterWidget::ResetListBoxAndDeleteButton()
{
	ListBox->ClearChildren();
	for (auto FxdgBindModel : BindModels)
	{
		UXDGUserCenterItemWidget * Item = UXDGUserCenterItemWidget::GenerateItem();
		Item->SetBindModel(FxdgBindModel);
		ListBox->AddChild(Item);
		Item->BindCallBack = [=](UXDGUserCenterItemWidget *CurrentWidget, TSharedPtr<FXUBindModel> Model)
		{
			if (Model->status == FXDGBindType::Bind)
			{
				enum UXDGUserCenterTipWidget::AlertType AlertType = GetBindCount() <= 1 ? UXDGUserCenterTipWidget::DeleteThird : UXDGUserCenterTipWidget::UnbindThird;
				UXDGUserCenterTipWidget::Show(AlertType, XUType::TapTap, [=]()
				{
					UnBind(CurrentWidget, Model);
				}, nullptr);
			} else
			{
				Bind(CurrentWidget, Model);
			}
		};
	}

	// if (userMd->loginType == (int)LoginType::Guest)
	// {
	// 	EmptyBox1->SetVisibility(ESlateVisibility::Visible);
	// 	DeleteButton->SetVisibility(ESlateVisibility::Visible);
	// } else
	// {
	// 	EmptyBox1->SetVisibility(ESlateVisibility::Collapsed);
	// 	DeleteButton->SetVisibility(ESlateVisibility::Collapsed);
	// }
}

void UXDGUserCenterWidget::ShouldShowErrorButton(bool Should)
{
	if (Should)
	{
		ListBox->SetVisibility(ESlateVisibility::Collapsed);
		EmptyBox2->SetVisibility(ESlateVisibility::Collapsed);
		ErrorButton->SetVisibility(ESlateVisibility::Visible);
	} else
	{
		ListBox->SetVisibility(ESlateVisibility::Visible);
		EmptyBox2->SetVisibility(ESlateVisibility::Visible);
		ErrorButton->SetVisibility(ESlateVisibility::Collapsed);
	}

	EmptyBox1->SetVisibility(ESlateVisibility::Collapsed);
	DeleteButton->SetVisibility(ESlateVisibility::Collapsed);
}

TArray<XULoginTypeModel> UXDGUserCenterWidget::GetSdkTypes()
{
	TArray<XULoginTypeModel> list;
	list.Add(XULoginTypeModel(XUType::TapTap));
	return list;
}

TArray<XULoginTypeModel> UXDGUserCenterWidget::GetSupportTypes()
{
	TArray<XULoginTypeModel> list;
	TArray<XULoginTypeModel> SDKList = GetSdkTypes();
	auto md = FXUInitConfigModel::GetLocalModel();
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

void UXDGUserCenterWidget::DeleteAccount(const FString& Tip)
{
	UTUHUD::ShowToast(Tip);
	XDUE::Logout();
	XDUE::ResetPrivacy();
	RemoveFromParent();
}

void UXDGUserCenterWidget::Bind(UXDGUserCenterItemWidget* CurrentWidget, TSharedPtr<FXUBindModel> Model)
{
	TFunction<void(TSharedPtr<FJsonObject> paras)> BindBlock = [=](TSharedPtr<FJsonObject> Paras)
	{
		UTUHUD::ShowWait();
		XUNet::Bind(Paras, [=](TSharedPtr<FXUResponseModel> ResponseModel, FXUError Error)
		{
			UTUHUD::Dismiss();
			if (ResponseModel.IsValid())
			{
				Model->status = (int) FXDGBindType::Bind;
				CurrentWidget->SetBindModel(Model);
				UTUHUD::ShowToast(langModel->tds_bind_success);
			} else
			{
				if (Error.code > 200)
				{
					UTUHUD::ShowToast(Error.msg);
				} else
				{
					UTUHUD::ShowToast(langModel->tds_bind_error);
				}
			}
			if (BindCallBack != nullptr) {
				TSharedPtr<FXUError> TempError = nullptr;
				if (!ResponseModel.IsValid()) {
					TempError = MakeShareable(new FXUError(Error));
				}
				int Type = Paras->GetNumberField("type");
				BindCallBack((XUType::LoginType)Type, TempError);
			}
		});
	};
	if (Model->loginType == (int)XUType::TapTap)
	{
		XUImpl::GetLoginParam(XUType::TapTap, [=](TSharedPtr<FJsonObject> Paras)
		{
			BindBlock(Paras);
		}, [=](FXUError error)
		{
			if (error.code ==  80081)
			{
				UTUHUD::ShowToast(langModel->tds_login_cancel);
			} else
			{
				UTUHUD::ShowToast(error.msg);
			}
		});
	}

	
}

void UXDGUserCenterWidget::UnBind(UXDGUserCenterItemWidget* CurrentWidget, TSharedPtr<FXUBindModel> Model)
{
	UTUHUD::ShowWait();
	XUNet::Unbind(Model->loginType, [=](TSharedPtr<FXUResponseModel> ResponseModel, FXUError Error)
	{
		UTUHUD::Dismiss();
		if (ResponseModel.IsValid())
		{
			if (GetBindCount() <= 1)
			{
				DeleteAccount(langModel->tds_unbind_delete_success_return_sign);
			} else
			{
				Model->status = (int) FXDGBindType::UnBind;
				CurrentWidget->SetBindModel(Model);
				UTUHUD::ShowToast(langModel->tds_unbind_success);
			}
		} else
		{
			if (Error.code > 200)
			{
				UTUHUD::ShowToast(Error.msg);
			} else
			{
				UTUHUD::ShowToast(langModel->tds_unbind_guest_return);
			}
		}
		if (UnbindCallBack != nullptr) {
			TSharedPtr<FXUError> TempError = nullptr;
			if (!ResponseModel.IsValid()) {
				TempError = MakeShareable(new FXUError(Error));
			}
			UnbindCallBack((XUType::LoginType)Model->loginType, TempError);
		}
	});

}

int UXDGUserCenterWidget::GetBindCount()
{
	int num = 0;
	for (auto FxdgBindModel : BindModels)
	{
		if (FxdgBindModel->status == FXDGBindType::Bind)
		{
			num++;
		}
	}
	return num;
}

