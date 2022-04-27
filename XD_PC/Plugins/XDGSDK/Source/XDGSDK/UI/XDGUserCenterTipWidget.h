
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "XDGUserCenterTipWidget.generated.h"


UCLASS()
class XDGSDK_API UXDGUserCenterTipWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UXDGUserCenterTipWidget(const FObjectInitializer& ObjectInitializer);

	// static void ShowPrivacy(TFunction<void(bool result)> Completed);

protected:

	virtual void NativeConstruct() override;

	// UFUNCTION()
	// void OnCheckStateChanged(bool isChecked);
	//
	// UFUNCTION()
	// void OnComfirmBtnClick();


private:
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleLabel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DetailLabel;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* TextBox;

	UPROPERTY(meta = (BindWidget))
	UButton* GreenButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GreenButtonLabel;

	UPROPERTY(meta = (BindWidget))
	UButton* WhiteButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WhiteButtonLabel;
	
	
	// TFunction<void(bool result)> Completed;
	
};