// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/TextBlock.h"
#include "XDGPrivacyWidget.generated.h"

/**
 *  
 */
UCLASS()
class XDGSDK_API UXDGPrivacyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	 UXDGPrivacyWidget(const FObjectInitializer& ObjectInitializer);

	static void ShowPrivacy(TFunction<void(bool result)> Completed);

protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCheckStateChanged(bool isChecked);

	UFUNCTION()
	void OnComfirmBtnClick();


private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleLabel;

	UPROPERTY(meta = (BindWidget))
	UButton* ComfirmButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ComfirmButtonLabel;

	UPROPERTY(meta = (BindWidget))
	UMultiLineEditableTextBox* PrivacyTextView1;

	UPROPERTY(meta = (BindWidget))
	UMultiLineEditableTextBox* PrivacyTextView2;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* AgreeCheckBox1;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* AgreeCheckBox2;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AgreeCheckLabel1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AgreeCheckLabel2;
	
	TFunction<void(bool result)> Completed;
	
};
