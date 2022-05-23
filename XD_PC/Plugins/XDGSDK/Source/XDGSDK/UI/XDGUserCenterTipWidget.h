
#pragma once

#include "CoreMinimal.h"
#include "XDEnumType.h"
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
	enum AlertType
	{
		DeleteGuest = 0,   //删除游客
		DeleteThird = 1,   //删除第三方
		UnbindThird = 2,   //解绑第三方
	};
	UXDGUserCenterTipWidget(const FObjectInitializer& ObjectInitializer);

	static void Show(AlertType AlertType, XDLoginType LoginType, TFunction<void()> SureCallBack, TFunction<void()> CancelCallBack);


protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnGreenBtnClick();

	UFUNCTION()
	void OnWhiteBtnClick();

	UFUNCTION()
	void OnTextBoxValueChange(const FText& Content);


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

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RedTipLabel;


	AlertType AlertType;
	XDLoginType LoginType;
	TFunction<void()> SureCallBack;
	TFunction<void()> CancelCallBack;

	void FirstStepUpdate();
	void SecondStepUpdate();

	enum Step
	{
		First,
		Second,
	};

	Step Step;

	UPROPERTY()
	UTexture2D * GrayTexture = nullptr;

	UTexture2D *&GetGrayTexture();
	
	UPROPERTY()
	UTexture2D * RedTexture = nullptr;

	UTexture2D *&GetRedTexture();
	
};