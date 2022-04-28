#include "TDUHUD.h"
// WidgetBlueprint'/TapCommon/BPTDUHUD.BPTDUHUD'

UTDUHUD* UTDUHUD::HUD = nullptr;

UTDUHUD*& UTDUHUD::GetCurrentHUD()
{
	if (HUD)
	{
		// HUD->GetWorld()->GetTimerManager().
		HUD->GetWorld()->GetTimerManager().ClearTimer(HUD->DismissTimerHandle);
		if (!HUD->IsInViewport())
		{
			HUD->AddToViewport();
		}
		return HUD;
	} else
	{
		if (UClass* MyWidgetClass = LoadClass<UTDUHUD>(nullptr, TEXT("WidgetBlueprint'/TapCommon/BPTDUHUD.BPTDUHUD_C'")))
		{
			if (GWorld && GWorld->GetWorld())
			{
        		HUD = CreateWidget<UTDUHUD>(GWorld->GetWorld(), MyWidgetClass);
				HUD->AddToViewport();
        	}
        }
		return HUD;
	}
}

UTDUHUD::UTDUHUD(const FObjectInitializer& ObjectInitializer)  : Super(ObjectInitializer)
{
}

void UTDUHUD::ShowWait()
{
	UTDUHUD* Current = GetCurrentHUD();
	Current->TitleLabel->SetVisibility(ESlateVisibility::Collapsed);
	Current->ImageBox->SetVisibility(ESlateVisibility::Visible);
	Current->BackView->SetVisibility(ESlateVisibility::Visible);
}

void UTDUHUD::ShowWaitAndToast(const FString& Toast)
{
	UTDUHUD* Current = GetCurrentHUD();
	Current->TitleLabel->SetVisibility(ESlateVisibility::Visible);
	Current->ImageBox->SetVisibility(ESlateVisibility::Visible);
	Current->TitleLabel->SetText(FText::FromString(Toast));
	Current->BackView->SetVisibility(ESlateVisibility::Visible);
}

void UTDUHUD::Dismiss()
{
	if (!HUD)
	{
		return;
	}
	HUD->GetWorld()->GetTimerManager().ClearTimer(HUD->DismissTimerHandle);
	if (HUD->IsInViewport())
	{
		HUD->RemoveFromParent();
	}
	
}

void UTDUHUD::ShowToast(const FString& Toast, float TimeInterval)
{
	UTDUHUD* Current = GetCurrentHUD();
	Current->TitleLabel->SetVisibility(ESlateVisibility::Visible);
	Current->ImageBox->SetVisibility(ESlateVisibility::Collapsed);
	Current->BackView->SetVisibility(ESlateVisibility::HitTestInvisible);
	Current->TitleLabel->SetText(FText::FromString(Toast));
	Current->GetWorld()->GetTimerManager().SetTimer(Current->DismissTimerHandle,
	[=]()
	{
		HUD->RemoveFromParent();
		HUD = nullptr;
	}, TimeInterval, false);
}

void UTDUHUD::NativeConstruct()
{
	Super::NativeConstruct();
	DismissTimerHandle.Invalidate();
}


