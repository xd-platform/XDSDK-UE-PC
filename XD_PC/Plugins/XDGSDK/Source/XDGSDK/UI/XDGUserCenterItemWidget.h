#pragma once

#include "CoreMinimal.h"
#include "XDGBindModel.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "XDGUserCenterItemWidget.generated.h"


UCLASS()
class XDGSDK_API UXDGUserCenterItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UXDGUserCenterItemWidget(const FObjectInitializer& ObjectInitializer);

	static UXDGUserCenterItemWidget * GenerateItem();

	void SetBindModel(const TSharedPtr<FXDGBindModel>& Model);

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
	UImage* TitleImage;

	UPROPERTY(meta = (BindWidget))
	UButton* BindButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BindLabel;

	UPROPERTY(meta = (BindWidget))
	UImage* ArrowImage;

	TSharedPtr<FXDGBindModel> BindModel;
	// TFunction<void(bool result)> Completed;
	
};