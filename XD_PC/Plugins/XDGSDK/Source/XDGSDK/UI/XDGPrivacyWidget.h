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
	UTextBlock* TitleLabel;
	UButton* ComfirmButton;
	UTextBlock* ComfirmButtonLabel;
	UMultiLineEditableTextBox* PrivacyTextView1;
	UMultiLineEditableTextBox* PrivacyTextView2;
	UCheckBox* AgreeCheckBox1;
	UCheckBox* AgreeCheckBox2;
	UTextBlock* AgreeCheckLabel1;
	UTextBlock* AgreeCheckLabel2;
	
	TFunction<void(bool result)> Completed;
	
};