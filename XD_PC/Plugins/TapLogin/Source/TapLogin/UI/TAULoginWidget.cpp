// Fill out your copyright notice in the Description page of Project Settings.
#include "TAULoginWidget.h"

#include "TAULoginLanguage.h"
#include "TAULoginNet.h"
#include "TDSHelper.h"
#include "TDUDebuger.h"


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
	
	HiddenRefreshButton();

	GetQrCode();

	WidgetIsClosed = false;
}

void UTAULoginWidget::OnCloseBtnClick()
{
	Close(TapAuthResult::CancelInit());
}

void UTAULoginWidget::OnRefreshBtnClick()
{
	HiddenRefreshButton();
	GetQrCode();
}

void UTAULoginWidget::OnJumpWebBtnClick()
{
	TDUDebuger::DisplayShow("OnJumpWebBtnClick");
}

void UTAULoginWidget::ShowRefreshButton()
{
	RefreshButton->SetVisibility(ESlateVisibility::Visible);
	QRCoverView->SetVisibility(ESlateVisibility::Visible);
}

void UTAULoginWidget::HiddenRefreshButton()
{
	RefreshButton->SetVisibility(ESlateVisibility::Hidden);
	QRCoverView->SetVisibility(ESlateVisibility::Hidden);
}

void UTAULoginWidget::ResetQrCode(const FString& Content)
{
	HiddenRefreshButton();
	auto texture = TDSHelper::GenerateQrCode(Content);
	QRImage->SetBrushFromTexture(texture);
}

void UTAULoginWidget::ShowTip(const FString& Tip, const FString& SubTip)
{
	TipLabel->SetText(FText::FromString(Tip));
	SubTipLabel->SetText(FText::FromString(SubTip));
	GetWorld()->GetTimerManager().ClearTimer(TipTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(TipTimerHandle,
		[=]()
		{
			TipLabel->SetText(FText::FromString(""));
			SubTipLabel->SetText(FText::FromString(""));
		}, 3.f, false);
}

void UTAULoginWidget::StartCheck()
{
	AsyncTask(ENamedThreads::AnyThread,[=]()
	{
		AutoCheck();
	});

}

void UTAULoginWidget::AutoCheck()
{
	if (!QrCodeModel.IsValid())
	{
		AsyncTask(ENamedThreads::GameThread, [=]()
		{
			ShowRefreshButton();
		});
		return;
	}
	int64 ExpireAt = FDateTime::UtcNow().ToUnixTimestamp() + QrCodeModel->expires_in;
	// int64 ExpireAt = FDateTime::UtcNow().ToUnixTimestamp() + 10;
	int64 LastCheckAt = 0;
	while (true)
	{
		FPlatformProcess::Sleep(0.5);
		if (WidgetIsClosed)
		{
			return;
		}
		int64 Now = FDateTime::UtcNow().ToUnixTimestamp();
		if (Now > ExpireAt) {
			AsyncTask(ENamedThreads::GameThread, [=]()
			{
				ShowRefreshButton();
			});
			return;
		}
		if (Now <= LastCheckAt + QrCodeModel->interval) {continue;}
		bool Wait = true;
		bool Stop = false;
		TFunction<void()> Event = nullptr;
		TAULoginNet::RequestAccessToken(QrCodeModel->device_code, [=, &Wait, &Stop, &Event](TSharedPtr<FTapAccessToken> Model, FTAULoginError Error)
		{
			if (Model.IsValid())
			{
				Event = [=]()
				{
					GetProfile(Model);
				};
				Stop = true;
			} else
			{
				if (Error.error == "authorization_pending")
				{
					
				} else if (Error.error == "authorization_waiting")
				{
					Event = [=]()
					{
						ShowTip(TAULoginLanguage::GetCurrentLang()->QrnNoticeSuccess(), TAULoginLanguage::GetCurrentLang()->QrnNoticeSuccess2());
					};
				} else if (Error.error == "access_denied")
				{
					Event = [=]()
					{
						ShowTip(TAULoginLanguage::GetCurrentLang()->QrNoticeCancel(), TAULoginLanguage::GetCurrentLang()->QrNoticeCancel2());
						GetQrCode();
					};
					Stop = true;
				} else if (Error.error == "invalid_grant")
				{
					Event = [=]()
					{
						ShowRefreshButton();
					};
					Stop = true;
				} else if (Error.error == "slow_down")
				{
					
				} else
				{
					Event = [=]()
					{
						ShowRefreshButton();
					};
					Stop = true;
				}
			}
			Wait = false;
		});
		// 有空改成信号量 FEvent
		while (Wait)
		{
			FPlatformProcess::Sleep(0.5);
		}
		if (WidgetIsClosed)
		{
			return;
		}
		LastCheckAt = FDateTime::UtcNow().ToUnixTimestamp();
		if (Event){
			AsyncTask(ENamedThreads::GameThread, Event);
		}
		if (Stop) {
			break;
		}
	}
}

void UTAULoginWidget::GetProfile(const TSharedPtr<FTapAccessToken>& AccessToken)
{
	TAULoginNet::RequestProfile(*AccessToken.Get(), [=](TSharedPtr<FTAUProfileModel> Model, FTAULoginError Error)
	{
		if (Model.IsValid())
		{
			AccessToken->SaveToLocal();
			Model->SaveToLocal();
			Close(TapAuthResult::SuccessInit(AccessToken));
		} else
		{
			FTapError TapError;
			TapError.code = Error.code;
			TapError.error_description = Error.error_description + "./t" + "Get profile error";
			Close(TapAuthResult::FailInit(TapError));
		}
	});
}

void UTAULoginWidget::Close(const TapAuthResult& Result)
{
	RemoveFromParent();
	if (Completed)
	{
		Completed(Result);
	}
	WidgetIsClosed = true;
}

void UTAULoginWidget::GetQrCode()
{
	TAULoginNet::RequestLoginQrCode(Permissions,
	[=](TSharedPtr<FTAUQrCodeModel> Model, FTAULoginError Error)
	{
		if (Model.IsValid())
		{
			QrCodeModel = Model;
			ResetQrCode(Model->qrcode_url);
			StartCheck();
			TDUDebuger::DisplayLog("QRCODE Get");
		} else
		{
			ShowRefreshButton();
			TDUDebuger::WarningLog("QRCODE Get Fail");
		}
	});
}



