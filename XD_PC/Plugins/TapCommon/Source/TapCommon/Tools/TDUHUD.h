#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "TDUHUD.generated.h"


UCLASS()
class TAPCOMMON_API UTDUHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UTDUHUD(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "UTDUHUD", meta = (DisplayName = "Show Wait"))
	static void ShowWait();
	
	UFUNCTION(BlueprintCallable, Category = "UTDUHUD", meta = (DisplayName = "Show Wait And Toast"))
	static void ShowWaitAndToast(const FString& Toast);
	
	UFUNCTION(BlueprintCallable, Category = "UTDUHUD", meta = (DisplayName = "Dismiss"))
	static void Dismiss();

	UFUNCTION(BlueprintCallable, Category = "UTDUHUD", meta = (DisplayName = "Show Toast"))
	static void ShowToast(const FString& Toast, float TimeInterval = 3.f);

	protected:

	virtual void NativeConstruct() override;
	
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleLabel;

	UPROPERTY(meta = (BindWidget))
	USizeBox* ImageBox;

	UPROPERTY(meta = (BindWidget))
	UOverlay* BackView;

private:

	static UTDUHUD* HUD;

	static UTDUHUD*& GetCurrentHUD();

	FTimerHandle DismissTimerHandle;
};