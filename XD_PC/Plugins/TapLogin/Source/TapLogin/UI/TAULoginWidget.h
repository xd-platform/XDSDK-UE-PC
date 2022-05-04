// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TapAuthResult.h"
#include "TAUQrCodeModel.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "TAULoginWidget.generated.h"

/**
 *  
 */
UCLASS()
class TAPLOGIN_API UTAULoginWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UTAULoginWidget(const FObjectInitializer& ObjectInitializer);

	static void ShowLoginUI(TArray<FString> Permissions, TFunction<void(TapAuthResult result)> Completed);

protected:

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnCloseBtnClick();

	UFUNCTION()
	void OnRefreshBtnClick();

	UFUNCTION()
	void OnJumpWebBtnClick();


private:
	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	UPROPERTY(meta = (BindWidget))
	UButton* RefreshButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JumpWebButton;

	UPROPERTY(meta = (BindWidget))
	UBorder* QRCoverView;

	UPROPERTY(meta = (BindWidget))
	UImage* QRImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleUseLabel; //return "使用";

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleLoginLabel; //return "账号登录";

	UPROPERTY(meta = (BindWidget))
	UTextBlock* QrTitleLoginLabel; //return "安全扫码登录";

	UPROPERTY(meta = (BindWidget))
	UTextBlock* QrRefreshLabel; //return "点击刷新";

	UPROPERTY(meta = (BindWidget))
	UTextBlock* QrNoticeUseLabel; //return "请使用";

	UPROPERTY(meta = (BindWidget))
	UTextBlock* QrNoticeClientLabel; //return "客户端";

	UPROPERTY(meta = (BindWidget))
	UTextBlock* QrNoticeScanToLoginLabel; //return "扫描二维码登录";

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WebLoginLabel; //return "使用网页浏览器完成授权";

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WebNoticeLabel; //return "点击下方按钮前往网页浏览器，\n授权 TapTap 账号，完成后将自动返回游戏。";
        
	UPROPERTY(meta = (BindWidget))
	UTextBlock* WebButtonJumpToWebLabel; //return "跳转至 TapTap";

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TipLabel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SubTipLabel;

private:
	
	TFunction<void(TapAuthResult result)> Completed;
	
	TArray<FString> Permissions;

	TSharedPtr<FTAUQrCodeModel> QrCodeModel = nullptr;

	FTimerHandle TipTimerHandle;
	
	FEvent* WaitEvent;

	bool WidgetIsClosed = true;

	void ShowRefreshButton();
	void HiddenRefreshButton();
	void ResetQrCode(const FString& Content);
	void ShowTip(const FString& Tip, const FString& SubTip);
	void StartCheck();
	void AutoCheck();
	void GetProfile(const TSharedPtr<FTapAccessToken>& AccessToken);
	void Close(const TapAuthResult& Result);

	void GetQrCode();

	
};
