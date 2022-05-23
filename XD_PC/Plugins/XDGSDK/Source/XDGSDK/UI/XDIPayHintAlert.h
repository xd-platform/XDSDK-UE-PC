#pragma once

#include "CoreMinimal.h"
#include "XDEnumType.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "XDIPayHintAlert.generated.h"


UCLASS()
class XDGSDK_API UXDIPayHintAlert : public UUserWidget
{
	GENERATED_BODY()

public:
	UXDIPayHintAlert(const FObjectInitializer& ObjectInitializer);

	static void Show(XDCheckPayType CheckType);

protected:
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnContactCustomerServiceBtnClick();

private:
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleLabel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DetailLabel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ContentLabel;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TailLabel;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ButtonLabel;

	UPROPERTY(meta = (BindWidget))
	UButton* ContactCustomerServiceButton;

	XDCheckPayType CheckType = None;
	
};

