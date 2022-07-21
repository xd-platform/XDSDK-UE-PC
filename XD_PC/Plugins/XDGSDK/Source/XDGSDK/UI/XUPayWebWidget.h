#pragma once

#include "CoreMinimal.h"
#include "XUType.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "TUWebBrowser.h"
#include "XUPayWebWidget.generated.h"


UCLASS()
class XDGSDK_API UXUPayWebWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UXUPayWebWidget(const FObjectInitializer& ObjectInitializer);

	static void Show(const FString& PayUrl, TFunction<void(XUType::PayResult Result)> CallBack);

protected:
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCloseClick();

	UFUNCTION()
	void OnUrlChanged(const FText& Text);
	

private:
	
	UPROPERTY(meta = (BindWidget))
	UTUWebBrowser* PayWebBrowser;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	TFunction<void(XUType::PayResult Result)> CallBack;

	FString PayUrl;

	void CloseWithResult(XUType::PayResult Result);
	
};

