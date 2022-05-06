// Fill out your copyright notice in the Description page of Project Settings.


#include "XDGPrivacyWidget.h"

#include "InitConfigModel.h"
#include "LanguageManager.h"
#include "TDSHelper.h"


UXDGPrivacyWidget::UXDGPrivacyWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UXDGPrivacyWidget::ShowPrivacy(TFunction<void(bool result)> Completed)
{
	if (UClass* MyWidgetClass = LoadClass<UXDGPrivacyWidget>(nullptr, TEXT("WidgetBlueprint'/XDGSDK/BPPrivacyUI.BPPrivacyUI_C'")))
	{
		if (GWorld && GWorld->GetWorld())
		{
			auto widget = CreateWidget<UXDGPrivacyWidget>(GWorld->GetWorld(), MyWidgetClass);
			widget->Completed = Completed;
			widget->AddToViewport();
		}
	}
}

void UXDGPrivacyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ComfirmButton->SetIsEnabled(false);
	
	AgreeCheckBox1->OnCheckStateChanged.AddUniqueDynamic(this, &UXDGPrivacyWidget::OnCheckStateChanged);
	AgreeCheckBox2->OnCheckStateChanged.AddUniqueDynamic(this, &UXDGPrivacyWidget::OnCheckStateChanged);

	FScriptDelegate BtnDel;
	BtnDel.BindUFunction(this, "OnComfirmBtnClick");
	ComfirmButton->OnClicked.Add(BtnDel);

	FInitConfigModel::GetPrivacyTxt(FInitConfigModel::GetLocalModel()->configs.serviceAgreementTxt,
	[=](FString txt)
	{
		if (PrivacyTextView1)
		{
			FormatTags(txt);
			txt = txt.Replace(TEXT("</b>"), TEXT("</>"));
			PrivacyTextView1->SetText(FText::FromString(txt));
		}
	});
	FInitConfigModel::GetPrivacyTxt(FInitConfigModel::GetLocalModel()->configs.serviceTermsTxt,
	[=](FString txt)
	{
		if (PrivacyTextView2)
		{
			FormatTags(txt);
			txt = txt.Replace(TEXT("</b>"), TEXT("</>"));
			PrivacyTextView2->SetText(FText::FromString(txt));
		}
		
	});
	auto langModel = LanguageManager::GetCurrentModel();
	TitleLabel->SetText(FText::FromString(langModel->tds_terms_agreement));
	ComfirmButtonLabel->SetText(FText::FromString(langModel->tds_confirm_agreement));
	AgreeCheckLabel1->SetText(FText::FromString(langModel->tds_service_terms_agreement));
	AgreeCheckLabel2->SetText(FText::FromString(langModel->tds_service_terms_agreement));
	
}

void UXDGPrivacyWidget::OnCheckStateChanged(bool isChecked)
{
	if (AgreeCheckBox1->IsChecked() && AgreeCheckBox2->IsChecked())
	{
		ComfirmButton->SetIsEnabled(true);
	} else
	{
		ComfirmButton->SetIsEnabled(false);
	}
}

void UXDGPrivacyWidget::OnComfirmBtnClick()
{
	FInitConfigModel::UpdatePrivacyState();
	if (Completed)
	{
		Completed(true);
	}
	RemoveFromParent();
}

void UXDGPrivacyWidget::FormatTags(FString& Content) {
	int index = 0;
	TArray<int> HeadTags;
	while (true) {
		index = Content.Find(TEXT("<b>"), ESearchCase::CaseSensitive, ESearchDir::FromStart, index);
		if (index == INDEX_NONE) {
			break;
		}
		HeadTags.Add(index);
		index += 3;
	}
	index = 0;
	TArray<int> TailTags;
	while (true) {
		index = Content.Find(TEXT("</b>"), ESearchCase::CaseSensitive, ESearchDir::FromStart, index);
		if (index == INDEX_NONE) {
			break;
		}
		TailTags.Add(index);
		index += 4;
	}
	int HeadIndex = 0;
	int TailIndex = 0;
	TArray<TArray<int>> Duis;
	TArray<TArray<int>> DuiStack;
	TMap<int, int> NeedDelete;
	while (!(HeadIndex == HeadTags.Num() && TailIndex == TailTags.Num())) {
		bool DealHead = false;
		if (HeadIndex == HeadTags.Num()) {
			DealHead = false;
		} else if (TailIndex == TailTags.Num()) {
			DealHead = true;
		} else {
			if (HeadTags[HeadIndex] < TailTags[TailIndex]) {
				DealHead = true;
			} else {
				DealHead = false;
			}
		}
		if (DealHead) {
			TArray<int> Dui;
			Dui.Add(HeadTags[HeadIndex]);
			DuiStack.Add(Dui);
			HeadIndex++;
		} else {
			if (DuiStack.Num() == 0) {
				NeedDelete.Add(TailTags[TailIndex], 4);
			} else {
				TArray<int> Dui = DuiStack.Last();
				Dui.Add(TailTags[TailIndex]);
				Duis.Add(Dui);
				DuiStack.RemoveAt(DuiStack.Num()-1);
			}
			TailIndex++;
		}
	}
	for (auto Stack : DuiStack) {
		for (auto TempIndex : Stack) {
			NeedDelete.Add(TempIndex, 3);
		}
	}
	for (int i = 0; i < Duis.Num() - 1; i++) {
		TArray<int> FirstDui = Duis[i];
		TArray<int> SecondDui = Duis[i+1];
		if (FirstDui[0] > SecondDui[0]) {
			NeedDelete.Add(FirstDui[0], 3);
			NeedDelete.Add(FirstDui[1], 4);
		}
	}
	NeedDelete.KeySort([](int First, int Second) {
		return First > Second;
	});
	for (auto Delete : NeedDelete) {
		Content.RemoveAt(Delete.Key, Delete.Value, false);
	}
}


