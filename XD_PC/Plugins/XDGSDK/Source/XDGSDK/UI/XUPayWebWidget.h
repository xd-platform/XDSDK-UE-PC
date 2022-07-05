#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "WebBrowserWidget/Public/WebBrowser.h"
#include "XUPayWebWidget.generated.h"


UCLASS()
class XDGSDK_API UXUPayWebWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UXUPayWebWidget(const FObjectInitializer& ObjectInitializer);

	static void Show();

protected:
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCloseClick();

	UFUNCTION()
	void OnUrlChanged(const FText& Text);
	

private:
	
	UPROPERTY(meta = (BindWidget))
	UWebBrowser* PayWebBrowser;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;
	
};

