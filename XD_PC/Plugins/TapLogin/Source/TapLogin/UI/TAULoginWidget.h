// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TapAuthResult.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/Image.h"
#include "Components/MultiLineEditableTextBox.h"
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
	UTextBlock* RefreshButtonLabel;

	UPROPERTY(meta = (BindWidget))
	UButton* JumpWebButton;

	UPROPERTY(meta = (BindWidget))
	UBorder* QRCoverView;

	UPROPERTY(meta = (BindWidget))
	UImage* QRImage;
	
	TFunction<void(TapAuthResult result)> Completed;
};