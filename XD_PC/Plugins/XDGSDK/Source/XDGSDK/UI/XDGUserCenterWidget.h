#pragma once

#include "CoreMinimal.h"
#include "LanguageManager.h"
#include "XDGBindModel.h"
#include "XDGError.h"
#include "XULoginTypeModel.h"
#include "XDGUser.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "XDGUserCenterItemWidget.h"
#include "XDGUserCenterWidget.generated.h"


UCLASS()
class XDGSDK_API UXDGUserCenterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UXDGUserCenterWidget(const FObjectInitializer& ObjectInitializer);

	static void ShowWidget(TFunction<void(XUType::LoginType Type, TSharedPtr<FXDGError>)> BindCallBack, TFunction<void(XUType::LoginType Type, TSharedPtr<FXDGError>)> UnbindCallBack);

protected:
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCloseBtnClick();

	UFUNCTION()
	void OnCopyBtnClick();

	UFUNCTION()
	void OnErrorBtnClick();

	UFUNCTION()
	void OnDeleteBtnClick();

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

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ListBox;

	UPROPERTY(meta = (BindWidget))
	USizeBox* EmptyBox1;

	UPROPERTY(meta = (BindWidget))
	USizeBox* EmptyBox2;

	UPROPERTY(meta = (BindWidget))
	UButton* ErrorButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ErrorButtonLabel;
	
	
	
	// TFunction<void(bool result)> Completed;

	TSharedPtr<FXDGUser> userMd;
	TSharedPtr<FXULanguageModel> langModel;

	TFunction<void(XUType::LoginType Type, TSharedPtr<FXDGError>)> BindCallBack;
	TFunction<void(XUType::LoginType Type, TSharedPtr<FXDGError>)> UnbindCallBack;

	FString GetLoginTypeName();

	void RequestList();

	void ResetListBoxAndDeleteButton();

	void ShouldShowErrorButton(bool Should);

	TArray<XULoginTypeModel> GetSdkTypes();

	TArray<XULoginTypeModel> GetSupportTypes();

	TArray<TSharedPtr<FXDGBindModel>> BindModels;

	void DeleteAccount(const FString& Tip);

	int GetBindCount();

	void Bind(UXDGUserCenterItemWidget *CurrentWidget, TSharedPtr<FXDGBindModel> Model);

	void UnBind(UXDGUserCenterItemWidget *CurrentWidget, TSharedPtr<FXDGBindModel> Model);

	

	
};

