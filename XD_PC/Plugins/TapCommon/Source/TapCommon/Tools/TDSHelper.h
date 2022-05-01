#pragma once
#include "Blueprint/UserWidget.h"

class TAPCOMMON_API TDSHelper
{
public:
	
	template <class UIType>
	static UIType* GetWidgetUI(UUserWidget *widget, FName name)
	{
		UIType* view = Cast<UIType>(widget->GetWidgetFromName(name));
		if (view == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s cant find"), *name.ToString());
		}
		return view;
	}

	static UTexture2D* GenerateQrCode(const FString& string);


	static FString GetRandomStr(int len);

	static void GenerateBrush(FSlateBrush& Brush, const FString& TexturePath);

	// 把Adder加到Object中。
	static void JsonObjectAppend(TSharedPtr<FJsonObject>& Object, const TSharedPtr<FJsonObject>& Adder);

};
