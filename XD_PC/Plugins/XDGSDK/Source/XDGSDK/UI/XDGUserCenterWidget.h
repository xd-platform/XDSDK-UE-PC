#pragma once

#include "CoreMinimal.h"
#include "LanguageManager.h"
#include "XDGUser.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "XDGUserCenterWidget.generated.h"


UCLASS()
class XDGSDK_API UXDGUserCenterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UXDGUserCenterWidget(const FObjectInitializer& ObjectInitializer);

	static void ShowWidget();

protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCloseBtnClick();

	UFUNCTION()
	void OnCopyBtnClick();


private:
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleLabel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* InfoTitleLabel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentLoginTitleLabel;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* IDTitleLabel;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BindInfoTitleLabel;

	UPROPERTY(meta = (BindWidget))
	UButton* CopyButton;

	UPROPERTY(meta = (BindWidget))
	UButton* DeleteButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DeleteButtonLabel;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;
	
	
	// TFunction<void(bool result)> Completed;

	TSharedPtr<FXDGUser> userMd;
	TSharedPtr<FLanguageModel> langModel;

	FString GetLoginTypeName();

	void RequestList();

	
};
