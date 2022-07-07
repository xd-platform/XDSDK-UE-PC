// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "XUPrivacyWidget.generated.h"

/**
 *  
 */
UCLASS()
class XDGSDK_API UXUPrivacyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	 UXUPrivacyWidget(const FObjectInitializer& ObjectInitializer);

	static void ShowPrivacy(TFunction<void()> Completed);

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
	URichTextBlock* PrivacyTextView1;

	UPROPERTY(meta = (BindWidget))
	URichTextBlock* PrivacyTextView2;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* AgreeCheckBox1;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* AgreeCheckBox2;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AgreeCheckLabel1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AgreeCheckLabel2;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* AdditionalCheckBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AdditionalCheckLabel;
	
	TFunction<void()> Completed;

	void FormatTags(FString& Content);


	bool IsInKrAndPushEnable();

	bool IsInNorthAmerica();
	
};
