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

	// parent 需要传入一个UObject*来保持生命周期，一般传入要设置的image就行了。
	static UTexture2D* GenerateQrCode(UObject* parent, const FString& string);


	static FString GetRandomStr(int len);
};
