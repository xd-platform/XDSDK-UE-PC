#pragma once

#include "CoreMinimal.h"
#include "XUType.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "DemoFirstWidget.generated.h"


UCLASS()
class UDemoFirstWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UDemoFirstWidget(const FObjectInitializer& ObjectInitializer);


protected:
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnInitButtonClick();
	
	UFUNCTION()
	void OnAnonymousLoginBtnClick();
	
	UFUNCTION()
	void OnTapTapLoginBtnClick();
	
	UFUNCTION()
	void OnAutoLoginBtnClick();
	
	UFUNCTION()
	void OnLogoutBtnClick();
	
	UFUNCTION()
	void OnResetPrivacyBtnClick();
	
	UFUNCTION()
	void OnUserCenterBtnClick();
	
	UFUNCTION()
	void OnFillingUIBtnClick();
	
	UFUNCTION()
	void OnCheckFillingBtnClick();
	
	UFUNCTION()
	void OnCustomerCenterBtnClick();
	
	UFUNCTION()
	void OnPushEnableBtnClick();
	
	UFUNCTION()
	void OnMomentBtnClick();
	
	UFUNCTION()
	void OnWebPayBtnClick();
	
	UFUNCTION()
	void OnEnvironmentBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	
	UFUNCTION()
	void OnRegionBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnLangBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnTestButtonClick();

	UFUNCTION()
	void OnGoogleLoginBtnClick();

	UFUNCTION()
	void OnAccountCancelBtnClick();

private:
	
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* EnvironmentBox;
	
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* RegionBox;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* LangBox;
	
	UPROPERTY(meta = (BindWidget))
	UButton* InitButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* AnonymousLoginBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* TapTapLoginBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* AutoLoginBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* LogoutBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* ResetPrivacyBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* UserCenterBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* FillingUIBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* CheckFillingBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* CustomerCenterBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* PushEnableBtn;
	
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* MomentTF;
	
	UPROPERTY(meta = (BindWidget))
	UButton* MomentBtn;
	
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* WebPayServerIDTF;
	
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* WebPayRoleIDTF;
	
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* WebPayProductIDTF;
	
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* WebPayProductNameTF;
	
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* WebPayAmountTF;
	
	UPROPERTY(meta = (BindWidget))
	UButton* WebPayBtn;

	UPROPERTY(meta = (BindWidget))
	UButton* TestButton;

	UPROPERTY(meta = (BindWidget))
	UButton* GoogleLoginBtn;

	UPROPERTY(meta = (BindWidget))
	UButton* AccountCancelBtn;
	
};

